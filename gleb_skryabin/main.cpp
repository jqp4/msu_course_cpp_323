#include <array>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "graphGenerator.hpp"

void writeGraphJSON(const Graph& graph, const std::string& filename) {
  std::ofstream json(filename);
  if (json.is_open()) {
    json << graph.toJSON();
    json.close();
    std::cout << "Graph is written to " << filename << std::endl;
  } else {
    throw std::runtime_error("Unable to open file");
  }
}

int main() {
  int inputDepth = 7;
  int newVerticesNum = 3;

  GraphGenerator generator =
      GraphGenerator(GraphGenerator::Params{inputDepth, newVerticesNum});
  const Graph graph = generator.generate();
  writeGraphJSON(graph, "graph.json");
  return 0;
}

// python3 vis/vis.py