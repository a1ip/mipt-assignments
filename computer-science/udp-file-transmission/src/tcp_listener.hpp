#ifndef UDP_FILE_TRANSMISSION_TCP_LISTENER_HPP_
#define UDP_FILE_TRANSMISSION_TCP_LISTENER_HPP_

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "tcp_socket.hpp"

class TcpListener {
 public:
  TcpListener();
  ~TcpListener();

  bool Bind(int port);

  bool Accept(TcpSocket* socket);

  bool Close();

 private:
  int socket_;
  sockaddr_in sockaddr_;

  enum {
    kAllocated,
    kBound,
    kClosed,
  } status_;
};

#endif /*UDP_FILE_TRANSMISSION_TCP_LISTENER_HPP_*/

