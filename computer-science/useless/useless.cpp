#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator = (const TypeName&)

// Splits the string 'source' using any of the characters that are part of 'delimeters'
// Stores the result in 'output'.
void split(const std::string& source, const std::string& delimeters, std::vector<std::string>* output) {
  output->clear();
  size_t read_till = 0;
  while(true) {
    size_t current_entry = source.find_first_of(delimeters, read_till);
    if(current_entry == std::string::npos)
      break;
    if(read_till != current_entry)
      output->push_back(source.substr(read_till, current_entry - read_till));
    read_till = current_entry + 1;
  }
  if(read_till != source.size())
    output->push_back(source.substr(read_till, source.size() - read_till));
}

// Executes command 'name' with 'arguments'.
// Returns PID of the executed process on success, returns -1 on error.
pid_t execute_command(const std::string& name, const std::vector<std::string>& arguments) {
  pid_t child_pid = fork();
  if(child_pid == 0) {
    char** c_arguments = new char* [arguments.size() + 2];
    if(c_arguments == NULL) {
      fprintf(stderr, "Error: unable to allocate memory while running '%s'!\n", name.c_str());
      abort();
    }
    c_arguments[0] = strdup(name.c_str());
    for(int i = 0; i < arguments.size(); i++) {
      c_arguments[i + 1] = strdup(arguments[i].c_str());
      if(c_arguments[i + 1] == NULL) {
        fprintf(stderr, "Error: unable to allocate memory while running '%s'!\n", name.c_str());
        abort();
      }
    }
    c_arguments[arguments.size() + 1] = NULL;

    execvp(c_arguments[0], c_arguments);
    fprintf(stderr, "Error: unable to run '%s'!\n", name.c_str());
    abort();
  }
  return child_pid;
}

// Reads the file to string, stores the result in 'output'.
// Returns true on success, returns false on error.
// Prints the error description, if one occures.
bool read_file_to_string(const std::string& filename, std::string* output) {  
  FILE* file = fopen(filename.c_str(), "r");
  if(file == NULL) {
    fprintf(stderr, "Error: while opening file %s for reading!\n", filename.c_str());
    return false;
  }

  output->clear();
  size_t buffer_size = 16;
  std::vector<char> buffer(buffer_size);
  while(true) {
    size_t bytes_read = fread(&buffer[0], sizeof(char), buffer_size, file);
    output->append(&buffer[0], bytes_read);
    if(bytes_read < buffer_size)
      break;
  }

  if(ferror(file) != 0) {
    fprintf(stderr, "Error: while reading file %s!\n", filename.c_str());
    output->clear();
    return false;
  }
  if(fclose(file) != 0) {
    fprintf(stderr, "Error: while closing file %s!\n", filename.c_str());
    output->clear();
    return false;
  }

  return true;
}

class Command {
 public:
  // Converts 'line' into a Command.
  // Returns pointer to a Command on success, returns NULL on error.
  // DOES NOT support ""-arguments.
  static Command* Create(const std::string& line) {
    std::auto_ptr<Command> result(new Command());
    if(result.get() == NULL)
      return NULL;

    std::vector<std::string> split_line;
    split(line, " \t", &split_line);
    if(split_line.size() < 2)
      return NULL;

    char* wrong_token = NULL;
    result->delay_ = strtol(split_line[0].c_str(), &wrong_token, 10) * 1000000;
    if(*wrong_token != '\0')
      return NULL;

    result->name_ = split_line[1];
    result->arguments_.assign(split_line.begin() + 2, split_line.end());
    return result.release();
  }

  const std::string& name() const { return name_; }
  const std::vector<std::string>& arguments() const { return arguments_; }
  unsigned int delay() const { return delay_; }

 private:
  std::string name_;
  std::vector<std::string> arguments_;
  unsigned int delay_;

  Command() { }
  DISALLOW_COPY_AND_ASSIGN(Command);
};

// Comparator, which is used to sort Commands by delay.
struct CommandComparator {
  bool operator () (const Command* left, const Command* right) const {
    return left->delay() < right->delay();
  }
};

// Reads commands from 'file', stores the result in 'output'.
// Returns true on success, returns false on error.
// Prints the error description, if one occures.
bool read_commands(const std::string& file, std::vector<Command*>* output) {
  std::string file_data;
  bool read_file_result = read_file_to_string(file, &file_data);
  if(read_file_result == false) {
    fprintf(stderr, "Error: while reading file '%s'!\n", file.c_str());
    return false;
  }
  std::vector<std::string> file_lines;
  split(file_data, "\n", &file_lines);

  output->clear();
  for(size_t i = 0; i < file_lines.size(); i++) {
    Command* current_command = Command::Create(file_lines[i]);
    if(current_command == NULL) {
      fprintf(stderr, "Error: while parsing line '%s'!\n", file_lines[i].c_str());
      for(size_t k = 0; k < output->size(); k++)
        delete (*output)[k];
      output->clear();
      return false;
    }
    output->push_back(current_command);
  }
  return true;
}

// Waits for all the children of the calling process.
// Returns true on success, returns false on error.
// Prints the error description, if one occures.
bool wait_children() {
  int status;
  while(true) {
    pid_t wait_result = wait(&status);
    if(wait_result == -1 && errno == ECHILD)
      break;
    if(wait_result == -1 || !WIFEXITED(status)) {
      fprintf(stderr, "Error: while waiting for a child!\n");
      return false;
    }
  }
  return true;
}

int main(int argc, char** argv) {
  if(argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::string file = argv[1];
  std::vector<Command*> commands;
  if(!read_commands(file, &commands)) {
    fprintf(stderr, "Error: while reading file '%s'!\n", file.c_str());
    return EXIT_FAILURE;
  }
  std::sort(commands.begin(), commands.end(), CommandComparator());

  useconds_t current_time = 0;
  for(size_t i = 0; i < commands.size(); i++) {
    if(usleep(commands[i]->delay() - current_time) == -1) {
      fprintf(stderr, "Error: while sleeping!\n");
      return EXIT_FAILURE;
    }
    current_time = commands[i]->delay();
    if(execute_command(commands[i]->name(), commands[i]->arguments()) == -1) {
      fprintf(stderr, "Error: while executing '%s'!\n", commands[i]->name().c_str());
      return EXIT_FAILURE;
    }
    delete commands[i];
  }

  if(!wait_children()) {
    fprintf(stderr, "Error: while waiting for the child processes to finish!\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

