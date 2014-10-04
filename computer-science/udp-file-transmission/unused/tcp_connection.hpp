#ifndef UDP_FILE_TRANSMISSION_TCP_CONNECTION_HPP_
#define UDP_FILE_TRANSMISSION_TCP_CONNECTION_HPP_

#include <string>

class TcpConnection {
 public:
  TcpConnection();
  ~TcpConnection();

  bool Initialize(const TcpSocket& send_socket, const TcpSocket& recv_socket);

  int Send(const char* data, size_t size);
  int Receive(char* data, size_t size);

  bool Close();

  enum {
    kAllocated,
    kInitialized,
    kClosed,
  } status_;

 private:
  TcpSocket send_socket_;
  TcpSocket write_socket_;
};

#endif /*UDP_FILE_TRANSMISSION_TCP_CONNECTION_HPP_*/

