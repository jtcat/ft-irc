#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "Server.hpp"

int main(int argc, char **argv) {
  int portno;
  std::string pass;
  if (argc != 3) {
    std::cerr << "Invalid number of arguments" << std::endl;
    return 1;
  }

  std::stringstream ss_port(argv[1]);
  std::stringstream ss_pass(argv[2]);

  ss_port >> portno;
  ss_pass >> pass;
  Server server(portno, pass);

  server.bind();
  server.listen(5);
  server.monitorClients();
  return 0;
}
