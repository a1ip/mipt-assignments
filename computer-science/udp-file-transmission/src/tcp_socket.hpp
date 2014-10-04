#ifndef UDP_FILE_TRANSMISSION_TCP_SOCKET_HPP_
#define UDP_FILE_TRANSMISSION_TCP_SOCKET_HPP_

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <string>

class TcpSocket {
 public:
  friend class TcpListener;

  TcpSocket();
  ~TcpSocket();

  bool Connect(const std::string& ip, int port, int attempts = 1);

  bool Send(const char* data, size_t size);
  bool Receive(char* data, size_t size);

  bool SendMessage(const std::string& message);
  bool ReceiveMessage(std::string* message);

  std::string GetIp() const;
  int GetPort() const;

  bool Close();

 private:
  int socket_;
  sockaddr_in sockaddr_;

  enum {
    kAllocated,
    kConnected,
    kClosed,
  } status_;
};

#endif /*UDP_FILE_TRANSMISSION_TCP_SOCKET_HPP_*/

