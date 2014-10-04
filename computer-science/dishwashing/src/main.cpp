#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "error.hpp"
#include "ipc_interface.hpp"
#include "utility.hpp"

typedef unsigned int uint32;

const std::string empty_message = "";

// Parses file, which consists of lines in the following format:
// <string> : <integer>
// For output format look at the declaration.
// Returns true on success, returns false on error.
bool ParseFile(const std::string& file, std::vector<std::pair<std::string, uint32> >* output) {
  output->clear();

  std::string data;
  if(!ReadFileToString(file, &data)) {
    printf("Error: could not read from file '%s'!\n", file.c_str());
    return false;
  }

  std::vector<std::string> lines;
  Split(data, "\n", &lines);

  for(size_t i = 0; i < lines.size(); i++) {
    std::vector<std::string> words;
    Split(lines[i], " \t", &words);

    int time;

    if(words.size() != 3 || words[1] != ":" || !ParseInt(words[2], &time) || time < 0) {
      fprintf(stderr, "Error: could not parse '%s' from '%s'!\n", lines[i].c_str(), file.c_str());
      return false;
    }

    output->push_back(std::make_pair(words[0], time));
  }

  return true;
}

// Parses file, which consists of lines in the following format:
// <string> : <integer>
// For output format look at the declaration.
// Returns true on success, returns false on error.
bool ParseFile(const std::string& file, std::map<std::string, uint32>* output) {
  output->clear();

  std::string data;
  if(!ReadFileToString(file, &data)) {
    printf("Error: could not read from file '%s'!\n", file.c_str());
    return false;
  }

  std::vector<std::string> lines;
  Split(data, "\n", &lines);

  for(size_t i = 0; i < lines.size(); i++) {
    std::vector<std::string> words;
    Split(lines[i], " \t", &words);

    int time;

    if(words.size() != 3 || words[1] != ":" || !ParseInt(words[2], &time) || time < 0) {
      fprintf(stderr, "Error: could not parse '%s'!\n", lines[i].c_str());
      return false;
    }

    (*output)[words[0]] = time;
  }

  return true;
}

bool producer_main(IpcInterface* ipc, const std::map<std::string, uint32>& production_time,
 const std::vector<std::pair<std::string, uint32> >& batch, pid_t consumer_pid) {
  if(!ipc->InitializeProducer()) {
    return false;
  }

  printf("Washer: ready!\n");

  for(std::vector<std::pair<std::string, uint32> >::const_iterator i = batch.begin(); i != batch.end(); ++i) {
    std::map<std::string, uint32>::const_iterator itr = production_time.find(i->first);
    if(itr == production_time.end()) {
      fprintf(stderr, "Error: no information about '%s'!\n", i->first.c_str());
      return false;
    }

    const std::string& product = itr->first;
    const uint32& time = itr->second;

    for(size_t product_number = 0; product_number < i->second; product_number++) {
      printf("Washer: now washing '%s' for %u seconds.\n", product.c_str(), time);
      if(sleep(time) != 0) {
        return false;
      }

      printf("Washer: looking if there is a free slot on the table...\n");

      std::string message;

      // Waiting for a free slot on the table.
      if(!ipc->ReceiveMessage(&message)) {
        return false;
      }

      if(message != empty_message) {
        fprintf(stderr, "Error: empty message should has been sent!\n");
        return false;
      }

      printf("Washer: putting '%s' on the table.\n", product.c_str());
      if(!ipc->SendMessage(product)) {
        return false;
      }
    }
  }

  printf("Washer: no work left.\n");
  if(!ipc->SendMessage(empty_message)) {
    return false;
  }

  int status, wait_ret;
  wait_ret = waitpid(consumer_pid, &status, 0);
  if(wait_ret != consumer_pid || !WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
    return false;
  }

  if(!ipc->Close()) {
    return false;
  }

  return true;
}

bool consumer_main(IpcInterface* ipc, size_t table_capacity, const std::map<std::string, uint32>& consuming_time) {
  if(!ipc->InitializeConsumer()) {
    return false;
  }

  // Tell producer that there are 'table_capacity' free slots on the table.
  for(size_t i = 0; i < table_capacity; i++) {
    if(!ipc->SendMessage(empty_message)) {
      return false;
    }
  }

  printf("Dryer: ready!\n");

  while(true) {
    printf("Dryer: looking for dishes...\n");

    std::string message;
    if(!ipc->ReceiveMessage(&message)) {
      return false;
    }

    if(message == empty_message) {
      break;
    }

    std::map<std::string, uint32>::const_iterator itr = consuming_time.find(message);
    if(itr == consuming_time.end()) {
      fprintf(stderr, "Error: no information about '%s'!\n", message.c_str());
      return false;
    }

    const std::string& product = itr->first;
    const uint32& time = itr->second;

    printf("Dryer: got a '%s', will be drying it for %u seconds.\n", product.c_str(), time);

    if(sleep(time) != 0) {
      return false;
    }

    printf("Dryer: finished the '%s'.\n", product.c_str());

    // Another slot has been freed.
    if(!ipc->SendMessage(empty_message)) {
      return false;
    }
  }

  printf("Dryer: no work left.\n");

  if(!ipc->Close()) {
    return false;
  }

  return true;
}

int main(int agrv, char** argc) {
  /***/const size_t table_capacity = 10;
  /***/IpcInterface::Type ipc_type = IpcInterface::kIpcFileBased;

  std::map<std::string, uint32> production_time, consuming_time;
  std::vector<std::pair<std::string, uint32> > batch;

  if(!ParseFile("data/producer.dat", &production_time)) {
    return EXIT_FAILURE;
  }
  if(!ParseFile("data/consumer.dat", &consuming_time)) {
    return EXIT_FAILURE;
  }
  if(!ParseFile("data/production.dat", &batch)) {
    return EXIT_FAILURE;
  }

  std::auto_ptr<IpcInterface> ipc(IpcInterface::Create(ipc_type));
  if(ipc.get() == NULL) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  pid_t consumer_pid = fork();
  if(consumer_pid == -1) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }
  if(consumer_pid == 0) {
    if(!consumer_main(ipc.get(), table_capacity, consuming_time)) {
      error::PrintLastErrorDescription();
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  if(!producer_main(ipc.get(), production_time, batch, consumer_pid)) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  // Producer will wait for consumer to maintain socket connection.

  if(!ipc->Close()) {
    error::PrintLastErrorDescription();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

