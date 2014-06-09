RailRoad
========

Table of Contents
---------
* [Introduction](#introduction)
* [Team Members](#team-members)

<a name="introduction"></a>Introduction
--------

RailRoad is a program designed and built to control model trains (that use LocoNet) at Western Washington University.

The main goal of the program was to create a program that would enforce separation among the trains. That is, ensure a user could not purposefully cause them to crash or derail.

The program was built to be modular.  That is, we wanted to ensure that if someone else wanted to use something we created, they could pick and choose what parts they wanted. We wanted to build the program so that there could be a multitude of possible interfaces, and ensure that it was at least partially secure.

With that in mind we created a few protocols for communication between this program and an API, and an API and a client.

We also believe in documentation.  Any code written that is intended to be re-used should be well documented.

Having said all of this, we did not meet our main goal.  Here is what we did accomplish:

- Track layout is not statically programmed in (except in the case of the Android app).
- Transmit to track.
- Receive from track and filter input.
- Documentation.
- Android App.
- API with simple authentication.

Features that need implementation:

- Initialization phase in Conductor.
- Transmitter power on uses static information for creating trains.
- Need to figure out position of train on track.
- Need to reserve sections based upon sensor ticks and train locations.
- If a train is locked, need to ensure it doesn't stay locked forever.
- Ensure Android App can deal with varying numbers of turnouts.

<a name="team-members"></a>Team Members:
---------
* Daniel Hansen
* James Sentinella
* Andrew Wear