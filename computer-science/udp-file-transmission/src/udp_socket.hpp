#ifndef UDP_FILE_TRANSMISSION_UDP_SOCKET_HPP_
#define UDP_FILE_TRANSMISSION_UDP_SOCKET_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>

class UdpSocket {
 public:
  UdpSocket();
  ~UdpSocket();

  bool Connect(const std::string& ip, int port);
  bool Bind(int port, bool block = false);

  int Send(const char* data, size_t size);
  int Receive(char* data, size_t size);

  bool Close();

 private:
  int socket_;
  sockaddr_in sockaddr_;

  enum {
    kAllocated,
    kConnected,
    kBound,
    kClosed,
  } status_;
};

#endif /*UDP_FILE_TRANSMISSION_UDP_SOCKET_HPP_*/

