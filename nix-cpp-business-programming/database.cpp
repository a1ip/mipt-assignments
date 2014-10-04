#include <cassert>
#include <cstdio>

#include <map>
#include <set>
#include <string>

std::map<int, std::map<int, int> > rights;
std::map<int, std::map<int, int> > lefts;

void Insert(int left, int right) {
  if (rights[left].count(right) == 0) {
    rights[left][right] = 1;
  } else {
    rights[left][right] += 1;
  }
  if (lefts[right].count(left) == 0) {
    lefts[right][left] = 1;
  } else {
    lefts[right][left] += 1;
  }
  assert(rights[left][right] == lefts[right][left]);
}

// type == 0: left
// type == 1: right
int Count(int type, int value) {
  if (type == 0) {
    int result = 0;
    std::map<int, int>::iterator i, begin = rights[value].begin(), end = rights[value].end();
    for (i = begin; i != end; ++i) {
      result += i->second;
    }
    return result;
  } else {
    int result = 0;
    std::map<int, int>::iterator i, begin = lefts[value].begin(), end = lefts[value].end();
    for (i = begin; i != end; ++i) {
      result += i->second;
    }
    return result;
  }
}

// type == 0: left
// type == 1: right
void Delete(int type, int value) {
  if (type == 0) {
    std::map<int, int>::iterator i, begin = rights[value].begin(), end = rights[value].end();
    for (i = begin; i != end; ++i) {
      lefts[i->first][value] -= i->second;
      if (lefts[i->first][value] == 0) {
        lefts[i->first].erase(value);
      }
    }
    rights[value].clear();
  } else {
    std::map<int, int>::iterator i, begin = lefts[value].begin(), end = lefts[value].end();
    for (i = begin; i != end; ++i) {
      rights[i->first][value] -= i->second;
      if (rights[i->first][value] == 0) {
        rights[i->first].erase(value);
      }
    }
    lefts[value].clear();
  }
}

int main() {


  char command[16];
  char type[16];

  int commands;
  scanf("%d", &commands);
  for (int i = 0; i < commands; i++) {
    scanf("%s", command);
    if (command[0] == 'A') {
      int left, right;
      scanf("%d %d", &left, &right);
      Insert(left, right);
      printf("%u\n", rights[left][right]);
    } else if (command[0] == 'C') {
      int value;
      scanf("%s %d", type, &value);
      if (type[0] == 'A') {
        printf("%u\n", Count(0, value));
      } else {
        printf("%u\n", Count(1, value));
      }
    } else if (command[0] == 'D') {
      int value;
      scanf("%s %d", type, &value);
      if (type[0] == 'A') {
        printf("%u\n", Count(0, value));
        Delete(0, value);
      } else {
        printf("%u\n", Count(1, value));
        Delete(1, value);
      }
    }
  }

  return 0;
}
