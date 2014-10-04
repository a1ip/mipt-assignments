#ifndef _IPC_SOCKET_BASED_HPP
#define _IPC_SOCKET_BASED_HPP

#include <string>

#include "ipc_interface.hpp"

class SocketBasedIpc : public IpcInterface {
 public:
  static SocketBasedIpc* Create();
  virtual ~SocketBasedIpc();

  virtual bool Close();

  virtual bool InitializeProducer();
  virtual bool InitializeConsumer();

  virtual bool SendMessage(const std::string& message);
  virtual bool ReceiveMessage(std::string* message);

 private:
  SocketBasedIpc();

  int AcceptConnection_(int port);
  int Connect_(const std::string& ip, int port);

  bool Send_(int socket, const char* data, size_t size);
  bool Receive_(int socket, char* data, size_t size);

  int sockets_[4];
  int send_socket_;
  int recv_socket_;

  enum {
    PARENT,
    PRODUCER,
    CONSUMER
  } status_;

  static const int producer_port_ = 51777;
  static const int consumer_port_ = 51778;
};

#endif /*_IPC_SOCKET_BASED_HPP*/

