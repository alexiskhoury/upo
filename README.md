# upo
Program to detect power outage and execute a script before properly shutting down the server

This a C++ program than can be launched from command line and can be run as a daemon process. The Program will monitor the network interface status connected to an Ethernet switch or router. On a power failure of the Ethernet switch or the router, the program can execute a script before shutting down the machine properly.

This program will log the vent to the syslog daemon. It is intended to be used on Linux servers power sourced by old UPSes that do not have a digital communication interface for safe shutdown procedure of the server.

# Usage: 

 -h  --help         Prints this help
 -d  --daemon       Daemon mode
 -f  --force        Force shutdown, execute 'shutdown -h now'  
 -i  --interface    Interface name e.g. eth0
 -s  --script       Full path of script to execute
 -t  --time         Time base in seconds
 
Sample command: ./ups_outage -i eth0 -d -f -t 120 -s /path/to/script.sh

# Installation
Use make all on the target machine to compile the program. A sample upsoutage.service file is given for automatic launch of the program on server startup. This is useful for servers using systemd daemons.