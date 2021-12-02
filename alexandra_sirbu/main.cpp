#include <fstream>
#include <iostream>
#include "graph.hpp"

constexpr int kVerticesCount = 14;

void writeGraphJSON(const Graph& graph, const std::string& filename) {
  std::ofstream json(filename);
  if (json.is_open()) {
    json << graph.toJSON();
    json.close();
    std::cout << "Done! :)\n";
  } else {
    std::runtime_error("Unable to open file :(");
  }
}

int main() {
  auto graph = Graph();

  for (int i = 0; i < kVerticesCount; i++) {
    graph.add_vertex();
  }

  graph.add_edge(0, 1); // 0
  graph.add_edge(0, 2);
  graph.add_edge(0, 3);
  graph.add_edge(1, 4);
  graph.add_edge(1, 5);
  graph.add_edge(1, 6);
  graph.add_edge(2, 7);
  graph.add_edge(2, 8);
  graph.add_edge(3, 9);
  graph.add_edge(4, 10);
  graph.add_edge(5, 10); //10
  graph.add_edge(6, 10);
  graph.add_edge(7, 11);
  graph.add_edge(8, 11);
  graph.add_edge(9, 12);
  graph.add_edge(10, 13);
  graph.add_edge(11, 13);
  graph.add_edge(12, 13); //17

  writeGraphJSON(graph, "graph.json");

  return 0;
}