#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
class Server {
 public:
  Server(int port);
  ~Server();
  void bind();
  void listen(int n);
  int accept();
  int getSockFd();
  void monitor_clients();
  void add_fd_ToPoll(int client_fd);
  void del_fd_FromPoll(int i);
  void broadcast(int sender_fd, char *msg, int nbytes);
  // int read(int client_sock, char* buffer, size_t size);

 private:
  int _sock_fd;
  struct sockaddr_in _serv_addr;
  std::vector<struct pollfd> _pfds;
  int _fd_count;
};

#endif /* ********************************************************** SOCKET_H \
        */
