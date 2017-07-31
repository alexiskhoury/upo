#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

// system headers

#include <sys/socket.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <csignal>
#include <syslog.h>
//#include <sys/types.h>
#include <sys/stat.h>

// user defined headers

#include "anyoption.h"

/*###########################################


functions declarations below this line


###########################################*/

// function returning IFF_RUNNING status

bool interface_status(char* interface);

// function implementing command line options

void parameters(int argc, char* argv[], char* iface, int& cycle_base, bool& daemonize, char* script, bool& force);

// signal handler function (terminate process)

void signal_handler(int signum);

// daemon function

void daemonize();

#endif
