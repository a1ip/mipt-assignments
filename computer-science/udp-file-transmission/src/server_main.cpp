#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "tcp_listener.hpp"
#include "tcp_socket.hpp"
#include "udp_socket.hpp"

#include "chunk.hpp"
#include "error.hpp"
#include "utility.hpp"

const int server_tcp_port = 64247;
const int client_udp_port = 64248;

std::string positive = "yes";
std::string negative = "no";

bool handle_client(TcpSocket* client_tcp, UdpSocket* client_udp) {
  std::string client_name = client_tcp->GetIp();

  std::string filename;
  if(!client_tcp->ReceiveMessage(&filename)) {
    return false;
  }
  printf("%s: Client requested file '%s'.\n", client_name.c_str(), filename.c_str());

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  FILE* file = fopen(filename.c_str(), "r");
  ScopedFile scoped_file(file);

  if(file == NULL) {
    printf("%s: File not found, sending negative response.\n", client_name.c_str());
    if(!client_tcp->SendMessage(negative)) {
      return false;
    }
    return true;
  }

  if(!client_tcp->SendMessage(positive)) {
    return false;
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  size_t file_size;
  GetFileSize(filename, &file_size);
  if(!client_tcp->Send(reinterpret_cast<const char*>(&file_size), sizeof(file_size))) {
    return false;
  }

  size_t chunk_size;
  if(!client_tcp->Receive(reinterpret_cast<char*>(&chunk_size), sizeof(chunk_size))) {
    return false;
  }

  size_t chunks_number = file_size / chunk_size + (file_size % chunk_size == 0 ? 0 : 1);
  if(!client_tcp->Send(reinterpret_cast<const char*>(&chunks_number), sizeof(chunks_number))) {
    return false;
  }

  printf("%s: File '%s': %u bytes == %u chunk(s) %u byte(s) each.\n", client_name.c_str(),
    filename.c_str(), file_size, chunks_number, chunk_size);

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  printf("%s: Starting hash verification...\n", client_name.c_str());

  Chunk current_chunk;
  std::string hash;

  for(size_t i = 0; i < chunks_number; i++) {
    current_chunk.number = i;
    if(!LoadChunk(file, file_size, &current_chunk)) {
      return false;
    }
    if(!GetSHA1Hash(current_chunk.data, chunk_size, &hash)) {
      return false;
    }
    printf("%s: Chunk %u, SHA1 = '%s'.\n", client_name.c_str(), i, hash.c_str());

    if(!client_tcp->SendMessage(hash)) {
      return false;
    }
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  printf("%s: Starting file transmission...\n", client_name.c_str());

  int temp = 0;

  while(true) {
    if(!client_tcp->Receive(reinterpret_cast<char*>(&current_chunk.number), sizeof(current_chunk.number))) {
      return false;
    }

    // assuming chunk number is less than max value of size_t
    if(current_chunk.number == static_cast<size_t>(-1)) {
      printf("%s: Got a termination signal.\n", client_name.c_str());
      break;
    }

    printf("%s: Chunk %u was requsted.\n", client_name.c_str(), current_chunk.number);

    if(!LoadChunk(file, file_size, &current_chunk)) {
      return false;
    }

    if(temp % 2 != 0) {
      if(client_udp->Send(reinterpret_cast<const char*>(&current_chunk), sizeof(current_chunk)) == -1) {
        return false;
      }
    }
    temp++;

    printf("%s: Chunk %u was sent.\n", client_name.c_str(), current_chunk.number);

    // TODO: throttling
    if(usleep(1000) == -1) {
      return false;
    }
  }

  printf("%s: File has been trasmitted.\n", client_name.c_str());

  return true;
}

int main() {
  TcpListener listener;

  if(!listener.Bind(server_tcp_port)) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  for(int i = 0; i < 2; i++) {
    TcpSocket client_tcp;

    if(!listener.Accept(&client_tcp)) {
      error::PrintLastErrorDescription();
      return EXIT_FAILURE;
    }

    printf("Established TCP connection with '%s:%d'.\n", client_tcp.GetIp().c_str(), client_tcp.GetPort());
  
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    UdpSocket client_udp;

    if(!client_udp.Connect(client_tcp.GetIp(), client_udp_port)) {
      error::PrintLastErrorDescription();
      return EXIT_FAILURE;
    }

    printf("Established UDP connection with '%s:%d'.\n", client_tcp.GetIp().c_str(), client_tcp.GetPort());

    printf("Handling client '%s:%d'.\n", client_tcp.GetIp().c_str(), client_tcp.GetPort());

    // TODO: fork
    if(!handle_client(&client_tcp, &client_udp)) {
      error::PrintLastErrorDescription();
      return EXIT_FAILURE;
    }

    printf("Client '%s:%d' was handled successfully.\n", client_tcp.GetIp().c_str(), client_tcp.GetPort());

    if(!client_udp.Close()) {
      error::PrintLastErrorDescription();
      return EXIT_FAILURE;
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    if(!client_tcp.Close()) {
      error::PrintLastErrorDescription();
      return EXIT_FAILURE;
    }
  }

  if(!listener.Close()) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

