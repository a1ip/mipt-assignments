#ifndef UDP_FILE_TRANSMISSION_CLIENT_HPP_
#define UDP_FILE_TRANSMISSION_CLIENT_HPP_

#include <string>

#include "tcp_connection.hpp"
#include "udp_connection.hpp"

class Client {
 public:
  Client();
  ~Client();

  bool Connect(const std::string& ip, int port);
  bool Disconnect();

  bool ReceiveFile(const std::string& file);

 private:
  std::string server_ip_;
  int server_port_;

  TcpConnection* tcp_;
  UdpConnection* udp_;

  enum {
    kCreated,
    kConnected,
    kDisconnected
  } status_;
};

#endif /*UDP_FILE_TRANSMISSION_CLIENT_HPP_*/

