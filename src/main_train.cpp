/*
   This file is part of RailRoad.

   RailRoad is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3 of the License.

   RailRoad is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with RailRoad.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <thread>
#include <atomic>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "MailCar.h"
#include "Logic.h"
#include "main_train.h"
#include "constants.h"

int main (int argc, char* argv[]) {
   std::atomic<bool>* ext = new atomic<bool>(false);
   int debug, dont_connect;

   //checks args for debug flag.
   parse_args(argc, argv, &debug, &dont_connect);

   char *execAPI[] = {(char*)UX_API, (char*)"-v", (char*)"12345", (debug? (char*)"-d" : 0) , 0};

   /* Set up socket for communication with API */
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      perror("Unable to open socket.");
   }
   struct sockaddr_in serv_addr, api_addr;
   bzero(&serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(PORTNO);
   int yes = 1;
   if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt:");
      exit(SOCK_SSO);
   }
   if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("bind:");
      exit(SOCK_BND);
   }
   if (listen(sockfd,5) < 0) {
      perror("listen:");
      exit(SOCK_LST);
   }
   cout<<"CND: "<<"Waiting for API to connect."<<endl;
   pid_t procid = fork();
   if (procid < 0) {
      perror("fork");
      exit(SOCK_FRK);
   } else if (!procid) {
      (void) execv(execAPI[0], execAPI);
      exit(0);
   }
   int apilen = sizeof(api_addr);
   int apisocket = accept(sockfd, (struct sockaddr*)&api_addr, (socklen_t*)&apilen);
   close(sockfd);

   /* Create message passers */
   MailCar toConductor(apisocket);
   MailCar toLogic;
   /* Create Track object */
   Track track;
   track.setup();
   //create SerialPort, Receiver, and Conductor.
   SerialPort usb(dont_connect);
   Receiver   rx(&usb, &toConductor, track.getNumSensors(), ext, debug);
   Conductor  tx(&usb, &toConductor, &toLogic, &track, apisocket, ext, debug);
   Logic      lg(&toConductor, &toLogic, &track, ext, debug);

   //start the Receiver and transmitter thread.
   std::thread rx_thrd(Receiver::start_thread, &rx);
   std::thread ct_thrd(Conductor::start_thread, &tx);
   std::thread lg_thrd(Logic::start_thread, &lg);

   //wait for all threads to complete
   ct_thrd.join();
   rx_thrd.join();
   lg_thrd.join();
   *ext = true;

   int status;
   waitpid(procid, &status, 0);
   //*free memory and exit
   delete ext;
   return 0;//*/
}

/** SETS DEBUG AND CONNECTIONLESS TO 1 IF "-d" OR "-c" FLAG IS IN COMMAND
    LINE ARGUMENTS. THIS SETS DEBUG AND CONNECTION FREE RESPECTIVELY IN PROGRAM. */
void parse_args(int argc, char* argv[], int* debug, int* connectionless) {
   *debug = *connectionless = 0;

   for(int i=1; i<argc; i++) {
      if(!strncmp("-d", argv[i], 2))
         *debug = 1;

      if(!strncmp("-c", argv[i], 2))
         *connectionless = 1;
   }
}
