#include <iostream>

#include <map>
#include <vector>
#include <string>

typedef std::vector<std::vector<size_t > > Graph;

bool DFS(const Graph& graph, size_t current, std::vector<int>* color, std::vector<size_t>* output) {
  (*color)[current] = 1;

  for (size_t i = 0; i < graph[current].size(); i++) {
    if ((*color)[graph[current][i]] == 0) {
      if (!DFS(graph, graph[current][i], color, output)) {
        return false;
      }
    } else if((*color)[graph[current][i]] == 1) {
      return false;
    }
  }

  (*color)[current] = 2;
  (*output).push_back(current);
  return true;
}

bool TopSort(const Graph& graph, std::vector<size_t>* output) {
  output->clear();

  size_t size = graph.size();
  std::vector<int> color(size, 0);

  for (size_t i = 0; i < size; i++) {
    if (color[i] == 0) {
      if (!DFS(graph, i, &color, output)) {
        return false;
      }
    }
  }

  return true;
}

int main() {
  size_t elements_count;
  std::cin >> elements_count;

  std::map<std::string, size_t> element_to_id;
  std::vector<std::string> id_to_element;

  for (size_t i = 0; i < elements_count; i++) {
    std::string current;
    std::cin >> current;
    element_to_id[current] = i;
    id_to_element.push_back(current);
  }
      
  Graph graph(elements_count);

  size_t dependencies_count;
  std::cin >> dependencies_count;

  for (size_t i = 0; i < dependencies_count; i++) {
    std::string first, second;
    std::cin >> first >> second;
    graph[element_to_id[second]].push_back(element_to_id[first]);
  }

  std::vector<size_t> top_sorted_list;

  if (TopSort(graph, &top_sorted_list)) {
    std::cout << "OK" << std::endl;
    size_t size = top_sorted_list.size();
    for (size_t i = 0; i < size; i++) {
      std::cout << id_to_element[top_sorted_list[i]] << std::endl;
    }
  } else {
    std::cout << "NO_SOLUTION" << std::endl;
  }

  return 0;
}
