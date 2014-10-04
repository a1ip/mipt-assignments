#ifndef UDP_FILE_TRANSMISSION_SERVER_HPP_
#define UDP_FILE_TRANSMISSION_SERVER_HPP_

#include "tcp_connection.hpp"
#include "udp_connection.hpp"

class Server {
 public:
  Server();
  ~Server();

  bool Initialize(int port);
  bool Run();
  bool Destroy();

 private:
  int port_;

  enum {
    kCreated,
    kInitialized,
    kRun,
    kStopped,
    kDestroyed
  } status_;
};

#endif /*UDP_FILE_TRANSMISSION_SERVER_HPP_*/

