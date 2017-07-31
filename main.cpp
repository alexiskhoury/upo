#include "functions.h"

using namespace std;

int main(int argc, char* argv[]) {
	// define local variables

	char ziface[10] = "eth0";
	char zscript[100] = "";
	string script = "";
	int cycle = 300;
	bool daemon_flag = false, response = false, halt = false;

	// register signals and signal_handler()

	signal(SIGINT, signal_handler);

	// parse command line options & intialize variables

	parameters(argc, argv, ziface, cycle, daemon_flag, zscript, halt);

	// append zscript to script

	script.append(zscript);

	if (daemon_flag == true) {
		daemonize();
	} else {
		//cout << "Process pid is: " << getpid() << endl;
		response = interface_status(ziface);
		cout << "Interface " << ziface << " online status is : " << response << endl;
	}

	// infinite loop

	while (daemon_flag) {
		response = interface_status(ziface);

		if (response == 0) {
			syslog(LOG_NOTICE, "Network interface is inactive");
			sleep(30);
			if (!interface_status(ziface)) {
				if (script.length() > 0) {
					syslog(LOG_NOTICE, "ups_outage will execute script");
					system(script.c_str());
				}
				if (halt) {
					syslog(LOG_NOTICE, "ups_outage will shutdown the system");
					system("shutdown -h now");
				}
			}
		}

		sleep(cycle);
	}

	closelog();

	return 0;
}
