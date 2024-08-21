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
  (void)argc;
  (void)argv;
  int portno;
  std::stringstream ss(argv[1]);
  ss >> portno;
  if (argc != 3) {
    std::cerr << "Invalid Numbe of args" << std::endl;
    return 1;
  }
  Socket server(portno);
  char buff[256];
  int newsock_fd, bytes_read, bytes_sent;
  server.bind();
  server.listen(5);
  newsock_fd = server.accept();
  std::cout << "Connection accepted.\n";
  bzero(buff, 256);
  bytes_read = recv(newsock_fd, buff, sizeof(buff), 0);
  std::cout << "MESSAGE: " << buff << std::endl;
  if (bytes_read < 0) {
    perror("ERROR reading from socket");
    exit(1);
  }
  bytes_sent = send(newsock_fd, "GOT YOUR MESSAGE\n", 18, 0);
  if (bytes_sent < 0) {
    perror("ERROR writing to socket");
    exit(1);
  }
  close(newsock_fd);
  return 0;
}
