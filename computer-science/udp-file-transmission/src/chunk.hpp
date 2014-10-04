#ifndef UDP_FILE_TRANSMISSION_CHUNK_HPP_
#define UDP_FILE_TRANSMISSION_CHUNK_HPP_

#include <stdio.h>
#include <stdlib.h>

struct Chunk {
  static const size_t chunk_size = 32 * 1024;

  size_t number;
  char data[chunk_size];
};

bool LoadChunk(FILE* file, size_t size, Chunk* chunk);

bool SaveChunk(FILE* file, size_t max_size, const Chunk* chunk);

#endif /*UDP_FILE_TRANSMISSION_CHUNK_HPP_*/

