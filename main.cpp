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

int main(int argc, char **argv) {
  // (void)argc;
  // (void)argv;
  int portno;
  std::stringstream ss(argv[1]);  // Convert string to int using stringstream
  ss >> portno;
  if (argc != 3) {
    std::cerr << "Invalid Numbe of args" << std::endl;
    return 1;
  }
  // need to parse input;

  // create listening socket
  int sock_fd, newsock_fd, bytes_read, bytes_sent;

  unsigned int addr_size;
  char buff[256];
  struct sockaddr_in serv_addr, client_addr;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }
  // init the sock_address struct to pass to bind
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  // bind the socket to the port passed as input
  int yes = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    perror("setsockopt");
    exit(1);
  }
  if (bind(sock_fd, reinterpret_cast<struct sockaddr *>(&serv_addr),
           sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }
  // start listening
  listen(sock_fd, 5);
  /* Accept actual connection from the client */
  addr_size = sizeof(client_addr);
  newsock_fd = accept(
      sock_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size);
  if (newsock_fd < 0) {
    perror("ERROR on accept");
    exit(1);
  }
  std::cout << "Connection accepted.\n";
  bzero(buff, 256);
  bytes_read = recv(newsock_fd, buff, sizeof(buff), 0);
  std::cout << "MESSAGE: " << buff << std::endl;
  if (bytes_read < 0) {
    perror("ERROR reading from socket");
    exit(1);
  }
  bytes_sent = send(newsock_fd, "GOT YOUR MESSAGE", 17, 0);
  if (bytes_sent < 0) {
    perror("ERROR writing to socket");
    exit(1);
  }
  close(newsock_fd);
  close(sock_fd);
}
