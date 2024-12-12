#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "Server.hpp"

bool	g_run = true;

void	sig_handler(int sig)
{
	(void)sig;

	g_run = false;
}

bool	validatePort(const std::string& portstr) {
	std::string::const_iterator	it = portstr.begin();

	while (it != portstr.end() && std::isdigit(*it)) {
		it++;
	}
	return it == portstr.end();
}

int main(int argc, char **argv) {
	int portno;
	std::string pass;
	if (argc != 3) {
		std::cerr << "Invalid number of arguments" << std::endl;
		return 1;
	}

	if (!validatePort(argv[1])) {
		std::cerr << "Invalid port" << std::endl;
		return 1;
	}

	std::stringstream ss_port(argv[1]);
	std::stringstream ss_pass(argv[2]);

	ss_port >> portno;
	ss_pass >> pass;

	signal(SIGINT, sig_handler);

	Server server(portno, pass);

	server.bind();
	server.listen(5);
	server.monitorClients();
	return 0;
}
