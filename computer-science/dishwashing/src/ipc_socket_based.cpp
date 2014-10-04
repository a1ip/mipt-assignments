#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <memory>
#include <string>
#include <vector>

#include "error.hpp"

#include "ipc_socket_based.hpp"

inline size_t min(size_t a, size_t b) {
  return a < b ? a : b;
}

SocketBasedIpc::SocketBasedIpc() {
  status_ = PARENT;
  for(size_t i = 0; i < 4; i++) {
    sockets_[i] = -1;
  }
}

SocketBasedIpc::~SocketBasedIpc() {
  Close();
}

SocketBasedIpc* SocketBasedIpc::Create() {
  std::auto_ptr<SocketBasedIpc> result(new SocketBasedIpc());
  if(result.get() == NULL) {
    error::ReportError(error::ERROR_MEMORY);
    return NULL;
  }

  return result.release();
}

bool SocketBasedIpc::Close() {
  bool result = true;

  for(size_t i = 0; i < 4; i++) {
    if(sockets_[i] != -1) {
      if(status_ == PARENT) {
        if(shutdown(sockets_[i], SHUT_RDWR) == -1) {
          result &= false;
        }
        if(close(sockets_[i]) == -1) {
          result &= false;
        }
      }
      sockets_[i] = -1;
    }
  }

  return result;
}

bool SocketBasedIpc::InitializeProducer() {
  status_ = PRODUCER;

  std::string localhost = "127.0.0.1";

  sockets_[0] = AcceptConnection_(producer_port_);
  if(sockets_[0] == -1) {
    return false;
  }

  sockets_[1] = Connect_(localhost, consumer_port_);
  if(sockets_[1] == -1) {
    return false;
  }

  recv_socket_ = sockets_[0];
  send_socket_ = sockets_[1];

  return true;
}

bool SocketBasedIpc::InitializeConsumer() {
  status_ = CONSUMER;

  std::string localhost = "127.0.0.1";

  sockets_[3] = Connect_(localhost, producer_port_);
  if(sockets_[3] == -1) {
    return false;
  }

  sockets_[2] = AcceptConnection_(consumer_port_);
  if(sockets_[2] == -1) {
    return false;
  }

  recv_socket_ = sockets_[2];
  send_socket_ = sockets_[3];

  return true;
}

bool SocketBasedIpc::SendMessage(const std::string& message) {
  size_t message_size = message.size();
  if(!Send_(send_socket_, reinterpret_cast<char*>(&message_size), sizeof(message_size))) {
    return false;
  }
  if(!Send_(send_socket_, message.c_str(), message.size())) {
    return false;
  }
  return true;
}

bool SocketBasedIpc::ReceiveMessage(std::string* message) {
  size_t message_size;
  if(!Receive_(recv_socket_, reinterpret_cast<char*>(&message_size), sizeof(message_size))) {
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

  if(!Receive_(recv_socket_, buffer, message_size)) {
    return false;
  }

  message->assign(buffer, message_size);

  return true;
}

int SocketBasedIpc::AcceptConnection_(int port) {
  int listen_socket = socket(PF_INET, SOCK_STREAM, 0);
  if(listen_socket == -1) {
    return -1;
  }

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(listen_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
    close(listen_socket);
    return -1;
  }

  if(listen(listen_socket, 1) == -1) {
    close(listen_socket);
    return -1;
  }

  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t client_addr_size = sizeof(client_addr);

  int client_socket = accept(listen_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_size);
  close(listen_socket);

  if(client_socket == -1) {
    return -1;
  }

  return client_socket;
}

int SocketBasedIpc::Connect_(const std::string& ip, int port) {
  int server_socket = socket(PF_INET, SOCK_STREAM, 0);
  if(server_socket == -1) {
    return -1;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());

  if(addr.sin_addr.s_addr == -1) {
    close(server_socket);
    return -1;
  }

  const int num_attempts = 2;
  int i;
  for (i = 0; i < num_attempts; i++) {
    if(connect(server_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != -1) {
      break;
    }
    sleep(1);
  }
  if(i == num_attempts) {
    close(server_socket);
    return -1;
  }

  return server_socket;
}

bool SocketBasedIpc::Send_(int socket, const char* data, size_t size) {
  size_t remaining_bytes = size;

  int sent_bytes = 0;
  while(remaining_bytes > 0 && (sent_bytes = send(send_socket_, data, remaining_bytes, 0)) > 0) {
    if(sent_bytes > remaining_bytes) {
      return false;
    }
    remaining_bytes -= sent_bytes;
    data += sent_bytes;
  }

  if(sent_bytes == -1) {
    return false;
  }

  return true;
}

bool SocketBasedIpc::Receive_(int socket, char* data, size_t size) {
  size_t remaining_bytes = size;

  const size_t buffer_size = 16;
  char buffer[buffer_size];

  int recv_bytes = 0;
  while(remaining_bytes > 0 && (recv_bytes = recv(recv_socket_, buffer, min(buffer_size, remaining_bytes), 0)) > 0) {
    memcpy(data, buffer, recv_bytes);
    data += recv_bytes;
    remaining_bytes -= recv_bytes;
  }

  if(recv_bytes == -1) {
    return false;
  }

  return true;
}

