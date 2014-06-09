#! /usr/bin/env python2.7
#
# WWU TrainLab API
# Fall 2013 through Spring 2014

#   This file is part of RailRoad.
#
#   RailRoad is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, version 3 of the License.
#
#   RailRoad is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with RailRoad.  If not, see <http://www.gnu.org/licenses/>.


import sys
import socket
import select
import re
import argparse
import string
import random

#Port For Train Communication
TPORT = 54321
THOST = "localhost"
#Port For Clients to Contact Server
SHOST = ''
SPORT = 54320
#Backlog for listen
BLOG  = 5
#Max size of data to be pulled in at a time
SIZE  = 2056

#Regular expressions for validating input.
RE_CHECKVALID = re.compile("(?P<match>[^\]]*\])")
RE_CHECKEXIT  = re.compile("\[term\]", re.IGNORECASE)
RE_CHECKCMD   = re.compile("\[(?P<command>[^\|\]]*)\|(?P<token>[^\|\]]*)\]")

# Array of Sockets associated with clients connected
inlist           = []
cons             = [sys.stdin]   # Array of all FD's to use select on
validatedClients = []
toBeValidated    = []

class nonValidatedClient:
	def __init__(self, socket, token):
		self.socket = socket
		self.token  = token.upper()

def token_gen(size=10, chars=string.ascii_letters+string.digits):
	return ''.join(random.choice(chars) for _ in range(size))

def main():
	keepAlive = 1;

	parser = argparse.ArgumentParser()
	parser.add_argument("-v", "--validation", dest="v", type=int, help="Validation token for giving to Train Controlling Software.")
	parser.add_argument("-d", "--debug", dest="d", action="store_true", help="Outputs debug messages.")
	parser.add_argument("-s", "--server", dest="s", type=str, default=THOST, help="Train Controlling Software IP address.")
	parser.add_argument("-p", "--port", dest="p", type=int, default=TPORT, help="Train Controlling Software port number.")
	args = parser.parse_args()

	debug = True if args.d == True else False

	try:
		# Create socket associated with train control software and connect.
		trainsock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		cons.append(trainsock)
		trainsock.connect((THOST,TPORT))
		print 'API: Socket Connected to ' + THOST + ' on port ' + "%s" %TPORT
		# Set up socket for Clients to connect
		apisock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		apisock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		apisock.bind((SHOST,SPORT))
		apisock.listen(BLOG)
		cons.append(apisock)     # Add to list of all connections
	except socket.error:
		print 'API: Failed to create sockets.'
		exit();

	while keepAlive:
		try:
			inready, outready, exceptready = select.select(cons, [], [])
		except select.error:
			print "API: Unable to use select statement."
			break
		for sock in inready:
			if sock == apisock:     # New client connection.
				try:
					client, address = apisock.accept()
					cons.append(client)
					inlist.append(client)
					clientToken = token_gen()
					toBeValidated.append(nonValidatedClient(client, clientToken))
					client.send("[ATOKEN|RQS]\n");
					print "API: Clients connected: " + str(len(inlist))
					print "API: Token for client is: " + clientToken
				except socket.error:
					print "API: Unable to accept client connection."
			elif sock == trainsock: # Train Controlling Software communicating with API
				somedata = sock.recv(SIZE)  # Throws away.
				if debug:
					print "API: Data received from Trains: " + somedata
				if len(somedata) == 0 or RE_CHECKEXIT.match(somedata):
					keepAlive = 0
					break
			elif sock == sys.stdin:
				line = sys.stdin.readline()
				if line == "quit\n" or line == "q\n" or line == "exit\n":
					trainsock.send("[ext]\n")
					keepAlive = 0
					break
			elif sock not in validatedClients:                          # Client hasn't be validated yet
				try:
					data = sock.recv(SIZE, socket.MSG_PEEK)
					newdata = data.replace('\n','').replace('\r','')
					if len(data) > 0 and newdata == '':					# Pull out newlines and linefeeds if that's all there.
						data = sock.recv(len(data))
					elif newdata:
						allMsg = RE_CHECKVALID.match(newdata)			# If ending bracket, get command
						if allMsg and allMsg.group("match"):
							data = sock.recv(len(allMsg.group("match")))
							newdata = data.replace('\n','').replace('\r','')
							cmdVal = RE_CHECKCMD.match(newdata)         # check if we received a proper command
							if cmdVal and cmdVal.group("command") and cmdVal.group("token"):
								thisToken = cmdVal.group("token")       # get the token supplied
								validToken = False
								if cmdVal.group("command").upper() == "CTOKEN":
									for client in toBeValidated:        # look through all the clients that haven't been validated
										if client.socket == sock and thisToken.upper() == client.token:
											validToken = True           # if they're the one sending and the token is correct
											toBeValidated.remove(client)# validate them
											validatedClients.append(sock)
											break
									if validToken == False:                 # send proper message based upon the above
										print "API: Invalid token received: " + thisToken
										sock.send("[ATOKEN|INV]\n")
									else:
										print "API: Valid token accepted."
										sock.send("[ATOKEN|ACC]\n")
							if debug:
								print "API: Client token should be: " + newdata
						elif len(newdata) > 512:                        # if we've gotten 512 characters, and no , then throw it all away
							data = sock.recv(SIZE)
					elif not data:
						close(sock)                                     # if they send us nothing, we close them
				except socket.error:                                    # on socket error, we close them
					close(sock)
					continue
			else:
				try:
					data = sock.recv(SIZE, socket.MSG_PEEK)
					newdata = data.replace('\n','').replace('\r','')
					if len(data) > 0 and newdata == '':					# Pull out newlines and linefeeds if that's all there.
						data = sock.recv(len(data))
					elif newdata:
						allMsg = RE_CHECKVALID.match(newdata)			# If ending bracket, pull it and send it out
						if allMsg and allMsg.group("match"):
							data = sock.recv(len(allMsg.group("match")))
							newdata = data.replace('\n','').replace('\r','')
							if debug:
								print "API: Passing along: " + newdata
							trainsock.send(newdata + "\n")
						elif len(newdata) > 512:                        # if we've gotten 512 characters, and no , then throw it all away
							data = sock.recv(SIZE)
					elif not data:
						close(sock)                                     # if they send us nothing, we close them
				except socket.error:                                    # on socket error, we close them
					close(sock)
					continue
	print "API: Following orders and shutting down."

# function that closes the socket given a connection (socket)
def close(con):
	con.close()
	if con in inlist:
		inlist.remove(con)
		print "API: Clients connected: " + str(len(inlist))
	if con in cons:
		cons.remove(con)

if __name__ == '__main__':
    main()