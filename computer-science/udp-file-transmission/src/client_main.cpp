#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <queue>
#include <vector>

#include "tcp_socket.hpp"
#include "udp_socket.hpp"

#include "chunk.hpp"
#include "error.hpp"
#include "utility.hpp"

const int server_tcp_port = 64247;
const int client_udp_port = 64248;

std::string positive = "yes";
std::string negative = "no";

bool download_file(TcpSocket* server_tcp, UdpSocket* server_udp, const std::string& filename) {
  printf("Sending request for file '%s'.\n", filename.c_str());
  if(!server_tcp->SendMessage(filename)) {
    return false;
  }

  std::string message;
  if(!server_tcp->ReceiveMessage(&message)) {
    return false;
  }
  if(message == negative) {
    printf("File '%s' not found.\n", filename.c_str());
    return true;
  }

  size_t file_size;
  if(!server_tcp->Receive(reinterpret_cast<char*>(&file_size), sizeof(file_size))) {
    return false;
  }

  size_t chunk_size = Chunk::chunk_size;
  if(!server_tcp->Send(reinterpret_cast<const char*>(&chunk_size), sizeof(chunk_size))) {
    return false;
  }

  size_t chunks_number;
  if(!server_tcp->Receive(reinterpret_cast<char*>(&chunks_number), sizeof(chunks_number))) {
    return false;
  }

  printf("File '%s': %u bytes == %u chunk(s) %u byte(s) each.\n",
    filename.c_str(), file_size, chunks_number, chunk_size);

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  std::string my_file = filename + ".dwn";

  size_t my_file_size;
  if(!GetFileSize(my_file, &my_file_size)) {
    return false;
  }
  printf("Current file size is %u.\n", my_file_size);

  FILE* file = NULL;
  if(my_file_size > file_size) {
    file = fopen(my_file.c_str(), "w+");
    my_file_size = 0;
  } else {
    file = fopen(my_file.c_str(), "a+");
  }
  ScopedFile scoped_file(file);

  if(file == NULL) {
    return false;
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  printf("Starting hash verification...\n");

  Chunk current_chunk;

  std::vector<bool> chunks_got(chunks_number, false);
  std::queue<size_t> chunks_to_get;
  size_t chunks_left = chunks_number;

  std::string hash;
  std::string server_hash;

  for(size_t i = 0; i < chunks_number; i++) {
    if(!server_tcp->ReceiveMessage(&server_hash)) {
      return false;
    }
    printf("Chunk %u, server SHA1 = '%s'.\n", i, server_hash.c_str());

    current_chunk.number = i;
    if(!LoadChunk(file, my_file_size, &current_chunk)) {
      return false;
    }
    if(!GetSHA1Hash(current_chunk.data, chunk_size, &hash)) {
      return false;
    }
    printf("Chunk %u, client SHA1 = '%s'.\n", i, hash.c_str());

    if(hash != server_hash) {
      printf("Bad chunk %u, gonna get it.\n", i);
      chunks_to_get.push(i);
    } else {
      printf("Chunk %u is fine.\n", i);
      chunks_got[i] = true;
      chunks_left--;
    }
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  printf("Starting file transmission. Gonna get %u chunks.\n", chunks_left);


  while(chunks_left > 0) {
    if(chunks_to_get.empty()) {
      for(size_t i = 0; i < chunks_number; i++) {
        if(!chunks_got[i]) {
          chunks_to_get.push(i);
        }
      }
    }

    assert(!chunks_to_get.empty());

    size_t next_chunk = chunks_to_get.front();
    chunks_to_get.pop();
    if(!server_tcp->Send(reinterpret_cast<const char*>(&next_chunk), sizeof(next_chunk))) {
      return false;
    }
    printf("Chunk %u was requsted.\n", next_chunk);

    int ret = server_udp->Receive(reinterpret_cast<char*>(&current_chunk), sizeof(current_chunk));

    if(ret == -1) {
      return false;
    }

    if(ret != 0) {
      if(!chunks_got[current_chunk.number]) {
        if(!SaveChunk(file, file_size, &current_chunk)) {
          return false;
        }
        chunks_left--;
        chunks_got[current_chunk.number] = true;
        printf("Chunk %u was received and saved.\n", current_chunk.number);
      } else {
        printf("Chunk %u was received and ignored.\n", current_chunk.number);
      }
    }

    // TODO: throttling
    if(usleep(1000) == -1) {
      return false;
    }
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  // assuming chunk number is less than max value of size_t
  size_t termination_signal = static_cast<size_t>(-1);
  if(!server_tcp->Send(reinterpret_cast<const char*>(&termination_signal), sizeof(termination_signal))) {
    return false;
  }

  printf("File has been trasmitted.\n");

  return true;
}

int main(int argc, char** argv) {
  if(argc != 3) {
    printf("Usage: %s <ip> <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::string server_ip = argv[1];
  std::string file = argv[2];

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  TcpSocket server_tcp;

  if(!server_tcp.Connect(server_ip, server_tcp_port)) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  printf("Established TCP connection with '%s:%d'.\n", server_tcp.GetIp().c_str(), server_tcp.GetPort());

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  UdpSocket server_udp;

  if(!server_udp.Bind(client_udp_port, false)) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  printf("Established UDP connection with '%s:%d'.\n", server_tcp.GetIp().c_str(), server_tcp.GetPort());

  if(!download_file(&server_tcp, &server_udp, file)) {
     error::PrintLastErrorDescription();
     return EXIT_FAILURE;
  }

  if(!server_udp.Close()) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  if(!server_tcp.Close()) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

