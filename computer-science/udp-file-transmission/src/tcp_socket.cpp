#include <assert.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <algorithm>
#include <string>
#include <vector>

#include "error.hpp"

#include "tcp_socket.hpp"

const int INVALID_SOCKET = -1;

TcpSocket::TcpSocket() {
  status_ = kAllocated;
  socket_ = INVALID_SOCKET;
  memset(&sockaddr_, 0, sizeof(sockaddr_));
}

TcpSocket::~TcpSocket() {
  //assert(status_ == kClosed);
  Close();
}

bool TcpSocket::Connect(const std::string& ip, int port, int attempts) {
  assert(status_ == kAllocated);

  socket_ = socket(PF_INET, SOCK_STREAM, 0);
  if(socket_ == INVALID_SOCKET) {
    return false;
  }

  sockaddr_.sin_family      = AF_INET;
  sockaddr_.sin_port        = htons(port);
  sockaddr_.sin_addr.s_addr = inet_addr(ip.c_str());

  if(sockaddr_.sin_addr.s_addr == -1) {
    return false;
  }

  int i;
  for(i = 0; i < attempts; i++) {
    if(connect(socket_, reinterpret_cast<sockaddr*>(&sockaddr_), sizeof(sockaddr_)) != -1) {
      break;
    }
    sleep(1);
  }
  if(i == attempts) {
    return false;
  }

  status_ = kConnected;
  return true;
}

bool TcpSocket::Send(const char* data, size_t size) {
  assert(status_ == kConnected);

  const char* to_send = data;
  int remaining_bytes = size;
  int sent_bytes;

  while(remaining_bytes > 0 && (sent_bytes = send(socket_, to_send, remaining_bytes, 0)) > 0) {
    remaining_bytes -= sent_bytes;
    to_send += sent_bytes;
  }

  if(sent_bytes == -1) {
    return false;
  }

  if(remaining_bytes > 0) {
    error::SetLastError(error::ERROR_TCP_SOCKET_CLOSED);
    return false;
  }

  return true;
}

bool TcpSocket::Receive(char* data, size_t size) {
  assert(status_ == kConnected);

  char* to_recv = data;
  int remaining_bytes = size;
  int recv_bytes;

  while(remaining_bytes > 0 && (recv_bytes = recv(socket_, to_recv, remaining_bytes, 0)) > 0) {
    remaining_bytes -= recv_bytes;
    to_recv += recv_bytes;
  }

  if(recv_bytes == -1) {
    return false;
  }

  if(remaining_bytes > 0) {
    error::SetLastError(error::ERROR_TCP_SOCKET_CLOSED);
    return false;
  }

  return true;
}

bool TcpSocket::SendMessage(const std::string& message) {
  size_t message_size = message.size();

  if(!Send(reinterpret_cast<char*>(&message_size), sizeof(message_size))) {
    return false;
  }

  if(!Send(message.c_str(), message.size())) {
    return false;
  }

  return true;
}

bool TcpSocket::ReceiveMessage(std::string* message) {
  size_t message_size;
  if(!Receive(reinterpret_cast<char*>(&message_size), sizeof(message_size))) {
    return false;
  }

  char static_buffer[128];
  std::vector<char> dynamic_buffer;
  char* buffer;

  if(message_size <= 128) {
    buffer = static_buffer;
  } else {
    dynamic_buffer.resize(message_size);
    buffer = &dynamic_buffer[0];
  }

  if(!Receive(buffer, message_size)) {
    return false;
  }

  message->assign(buffer, message_size);

  return true;
}

std::string TcpSocket::GetIp() const {
  return std::string(inet_ntoa(sockaddr_.sin_addr));
}

int TcpSocket::GetPort() const {
  return ntohs(sockaddr_.sin_port);
}

bool TcpSocket::Close() {
  //assert(status_ == kAllocated || status_ == kConnected);

  if(socket_ != INVALID_SOCKET) {
    if(close(socket_) == -1) {
      return false;
    }
    socket_ = INVALID_SOCKET;
  }

  status_ = kClosed;
  return true;
}



