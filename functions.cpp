#include "functions.h"

/*###########################################


 functions definitions below this line


 ###########################################*/

// function returning IFF_RUNNING status
bool interface_status(char* interface) {
	struct ifreq ifr;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, interface);
	if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
		perror("SIOCGIFFLAGS");
	}
	close(sock);
	return !!(ifr.ifr_flags & IFF_RUNNING);
}

// function implementing command line options

void parameters(int argc, char* argv[], char* iface, int& cycle_base, bool& daemonize, char* script, bool& force) {
	/* 1. CREATE AN OBJECT */
	AnyOption *opt = new AnyOption;

	/* 2. SET PREFERENCES  */
	//opt->noPOSIX(); /* do not check for POSIX style character options */
	//opt->setVerbose(); /* print warnings about unknown options */
	//opt->autoUsagePrint(true); /* print usage for bad options */
	/* 3. SET THE USAGE/HELP   */
	opt->addUsage("");
	opt->addUsage("Usage: ");
	opt->addUsage("");
	opt->addUsage(" -h  --help         Prints this help ");
	opt->addUsage(" -d  --daemon       Daemon mode ");
	opt->addUsage(" -f  --force        Force shutdown, execute 'shutdown -h now'  ");
	opt->addUsage(" -i  --interface    Interface name e.g. eth0 ");
	opt->addUsage(" -s  --script       Full path of script to execute ");
	opt->addUsage(" -t  --time         Time base in seconds ");
	opt->addUsage("");

	/* 4. SET THE OPTION STRINGS/CHARACTERS */

	/* by default all  options  will be checked on the command line and from option/resource file */
	opt->setFlag("help", 'h'); /* a flag (takes no argument), supporting long and short form */
	opt->setFlag("daemon", 'd');
	opt->setFlag("force", 'f');
	opt->setOption("interface", 'i'); /* an option (takes an argument), supporting long and short form */
	opt->setOption("time", 't');
	opt->setOption("script", 's');

	/* go through the command line and get the options  */
	opt->processCommandArgs(argc, argv);

	if (!opt->hasOptions()) /* print usage if no options */
	{
		opt->printUsage();
		delete opt;
		exit(0);
	}

	/* 6. GET THE VALUES */
	if (opt->getFlag("help") || opt->getFlag('h'))
		opt->printUsage();
	if (opt->getFlag("daemon") || opt->getFlag('d'))
		daemonize = true;
	if (opt->getFlag("force") || opt->getFlag('f'))
			force = true;
	if (opt->getValue('i') != NULL || opt->getValue("interface") != NULL) {
		//cout << "interface = " << opt->getValue( 'i' ) << endl ;
		strcpy(iface, opt->getValue('i'));
	}
	if (opt->getValue('t') != NULL || opt->getValue("time") != NULL) {
		//cout << "time = " << opt->getValue( "time" ) << endl ;
		cycle_base = atoi(opt->getValue('t'));
	}
	if (opt->getValue('s') != NULL || opt->getValue("script") != NULL) {
		//cout << "script = " << opt->getValue('s') << endl;
		strcpy(script, opt->getValue('s'));
	}

	/* 7. GET THE ACTUAL ARGUMENTS AFTER THE OPTIONS
	 for( int i = 0 ; i < opt->getArgc() ; i++ ){
	 cout << "arg = " <<  opt->getArgv( i ) << endl ;
	 } */

	/* 8. DONE */
	delete opt;
	return;
}

// signal handling function (ctrl-c)

void signal_handler(int signum) {
	// cleanup and close up stuff here
	// break loop or terminate program
	if (signum == 2) {
		closelog();
		exit(signum);
	}
}

// daemonize function

void daemonize() {
	// set logging daemon messages in /var/messages or /var/log/syslog
	//setlogmask(LOG_UPTO(LOG_NOTICE)); //did not work under ubuntu 17.04
	openlog("ups_outage_daemon", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Entering Daemon");

	pid_t pid, sid;

	//Fork the Parent Process
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	//We got a good pid, Close the Parent Process
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	//Change File Mask
	umask(0);

	//Create a new Signature Id for our child
	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	//Change Directory
	//If we cant find the directory we exit with failure.
	if ((chdir("/tmp")) < 0) {
		exit(EXIT_FAILURE);
	}

	//Close Standard File Descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
