#ifndef UDP_FILE_TRANSMISSION_UDP_CONNECTION_HPP_
#define UDP_FILE_TRANSMISSION_UDP_CONNECTION_HPP_

#include <string>

class UdpConnection {
 public:
  UdpConnection();
  ~UdpConnection();

  bool Accept(int port);
  bool Connect(const std::string& ip, int port);

  bool Send(const std::string& message);
  bool Receive(std::string* message);

  bool Close();

 private:
  int port_;
  std::string ip_;

  int socket_;
  sockaddr_in sockaddr_;

  enum {
    kCreated,
    kConnectionAccepted,
    kConnected,
    kClosed
  } status_;
};

#endif /*UDP_FILE_TRANSMISSION_UDP_CONNECTION_HPP_*/

