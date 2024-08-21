#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "Socket.hpp"

int main(int argc, char **argv) {

  int portno;
  std::stringstream ss(argv[1]);
  ss >> portno;
  if (argc != 3) {
    std::cerr << "Invalid Numbe of args" << std::endl;
    return 1;
  }
  Server server(portno);

  server.bind();
  server.listen(5);
  server.monitor_clients();
  return 0;
}
