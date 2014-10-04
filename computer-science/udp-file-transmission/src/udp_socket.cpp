#include <assert.h>
#include <errno.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>

#include "udp_socket.hpp"

const int INVALID_SOCKET = -1;

UdpSocket::UdpSocket() {
  status_ = kAllocated;
  socket_ = INVALID_SOCKET;
  memset(&sockaddr_, 0, sizeof(sockaddr_));
}

UdpSocket::~UdpSocket() {
  //assert(status_ == kClosed);
  Close();
}

bool UdpSocket::Connect(const std::string& ip, int port) {
  assert(status_ == kAllocated);

  socket_ = socket(PF_INET, SOCK_DGRAM, 0);
  if(socket_ == INVALID_SOCKET) {
    return false;
  }

  sockaddr_.sin_family      = AF_INET;
  sockaddr_.sin_port        = htons(port);
  sockaddr_.sin_addr.s_addr = inet_addr(ip.c_str());

  if(sockaddr_.sin_addr.s_addr == -1) {
    return false;
  }

  if(connect(socket_, reinterpret_cast<sockaddr*>(&sockaddr_), sizeof(sockaddr_)) == -1) {
    return false;
  }

  status_ = kConnected;
  return true;
}

bool UdpSocket::Bind(int port, bool block) {
  assert(status_ == kAllocated);

  socket_ = socket(PF_INET, SOCK_DGRAM, 0);
  if(socket_ == INVALID_SOCKET) {
    return false;
  }

  if(!block) {
    if(fcntl(socket_, F_SETFL, O_NONBLOCK) == -1) {
      return false;
    }
  }

  sockaddr_.sin_family      = AF_INET;
  sockaddr_.sin_port        = htons(port);
  sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);

  if(sockaddr_.sin_addr.s_addr == -1) {
    return false;
  }

  if(bind(socket_, reinterpret_cast<sockaddr*>(&sockaddr_), sizeof(sockaddr_)) == -1) {
    return false;
  }

  status_ = kBound;
  return true;
}


int UdpSocket::Send(const char* data, size_t size) {
  assert(status_ == kConnected);

  if(send(socket_, data, size, 0) != size) {
    return -1;
  }

  return 1;
}

int UdpSocket::Receive(char* data, size_t size) {
  assert(status_ == kBound);

  if(recv(socket_, data, size, 0) != size) {
    if(errno == EAGAIN || errno == EWOULDBLOCK) {
      return 0;
    }
    return -1;
  }

  return 1;
}

bool UdpSocket::Close() {
  //assert(status_ == kAllocated || status_ == kConnected || status_ == kBound);

  if(socket_ != INVALID_SOCKET) {
    if(close(socket_) == -1) {
      return false;
    }
    socket_ = INVALID_SOCKET;
  }

  status_ = kClosed;
  return true;
}

