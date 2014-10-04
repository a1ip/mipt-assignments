#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

void Split(const std::string& input, std::vector<std::string>& output) {
  std::string current;
  for (size_t i = 0; i < input.size(); i++) {
    if (isspace(input[i])) {
      if (current.size() > 0) {
        output.push_back(current);
        current.clear();
      }
    } else {
      current.push_back(input[i]);
    }
  }
  if (current.size() > 0) {
    output.push_back(current);
  }
}

int StrToInt(const std::string& input) {
  return atoi(input.c_str());
}

std::map<std::string, int> vars;
std::vector<std::string> commands;

size_t Inc(size_t start, bool really) {
  assert(commands[start] == "inc");

  if (really) {
    vars[commands[start + 1]] += 1;
  }
  return 2;
}

size_t Dec(size_t start, bool really) {
  assert(commands[start] == "dec");

  if (really) {
    vars[commands[start + 1]] -= 1;
  }
  return 2;
}

size_t Set(size_t start, bool really) {
  assert(commands[start] == "set");

  if (really) {
    vars[commands[start + 1]] = StrToInt(commands[start + 2]);
  }
  return 3;
}

size_t Move(size_t start, bool really) {
  assert(commands[start] == "move");

  if (really) {
    vars[commands[start + 1]] = vars[commands[start + 2]];
  }
  return 3;
}

size_t Print(size_t start, bool really) {
  assert(commands[start] == "print");

  if (really) {
    std::cout << vars[commands[start + 1]] << std::endl;
  }
  return 2;
}

size_t ExecuteOne(size_t start, bool really);

size_t If(size_t start, bool really) {
  assert(commands[start] == "if");

  return 2 + ExecuteOne(start + 2, really && vars[commands[start + 1]] != 0);
}

size_t While(size_t start, bool really) {
  assert(commands[start] == "while");

  while (really && vars[commands[start + 1]] != 0) {
    ExecuteOne(start + 2, true);
  }
  return 2 + ExecuteOne(start + 2, false);
}

size_t Begin(size_t start, bool really) {
  assert(commands[start] == "begin");

  int depth = 1;
  size_t end = start;
  while (depth > 0) {
    end += 1;
    if (commands[end] == "begin") {
      depth += 1;
    } else if (commands[end] == "end") {
      depth -= 1;
    }
  }

  size_t current = start + 1;
  while (current < end) {
    current += ExecuteOne(current, really);
  }
  assert(current == end);

  return end - start + 1;
}

size_t ExecuteOne(size_t start, bool really) {
  if (commands[start] == "inc") {
    return Inc(start, really);
  } else if (commands[start] == "dec") {
    return Dec(start, really);
  } else if (commands[start] == "set") {
    return Set(start, really);
  } else if (commands[start] == "move") {
    return Move(start, really);
  } else if (commands[start] == "if") {
    return If(start, really);
  } else if (commands[start] == "while") {
    return While(start, really);
  } else if (commands[start] == "print") {
    return Print(start, really);
  } else if (commands[start] == "begin") {
    return Begin(start, really);
  }
}

int main() {
  std::string line;
  while (std::cin) {
    std::getline(std::cin, line);
    Split(line, commands);
  }

  size_t length = ExecuteOne(0, false);

  size_t current = length;
  int n = StrToInt(commands[current]);
  current += 1;
  for (int i = 0; i < n; i++) {
    vars.clear();
    int m = StrToInt(commands[current]);
    current += 1;
    for (int j = 0; j < m; j++) {
      vars[commands[current]] = StrToInt(commands[current + 1]);
      current += 2;
    }
    ExecuteOne(0, true);
  }

  return 0;
}
