#include <assert.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "tcp_socket.hpp"

#include "tcp_listener.hpp"

const int INVALID_SOCKET = -1;

TcpListener::TcpListener() {
  status_ = kAllocated;
  socket_ = 0;
  memset(&sockaddr_, 0, sizeof(sockaddr_));
}

TcpListener::~TcpListener() {
  //assert(status_ == kClosed);
  Close();
}

bool TcpListener::Bind(int port) {
  assert(status_ == kAllocated);

  socket_ = socket(PF_INET, SOCK_STREAM, 0);
  if(socket_ == INVALID_SOCKET) {
    return false;
  }

  sockaddr_.sin_family      = AF_INET;
  sockaddr_.sin_port        = htons(port);
  sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(socket_, reinterpret_cast<sockaddr*>(&sockaddr_), sizeof(sockaddr_)) == -1) {
    return false;
  }

  if(listen(socket_, 1) == -1) {
    return false;
  }

  status_ = kBound;
  return true;
}

bool TcpListener::Accept(TcpSocket* socket) {
  assert(status_ == kBound);

  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t client_addr_size = sizeof(client_addr);

  int client_socket = accept(socket_, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_size);

  if(client_socket == INVALID_SOCKET) {
    return false;
  }

  socket->socket_ = client_socket;
  socket->sockaddr_ = client_addr;
  socket->status_ = TcpSocket::kConnected;

  return true;
}

bool TcpListener::Close() {
  //assert(status_ == kAllocated || status_ == kBound);

  if(socket_ != INVALID_SOCKET) {
    if(close(socket_) == -1) {
      return false;
    }
    socket_ = INVALID_SOCKET;
  }

  status_ = kClosed;
  return true;
}

