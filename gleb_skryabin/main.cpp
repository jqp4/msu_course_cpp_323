#include <fstream>
#include <iostream>
#include "graphGenerator.hpp"
#include "graphPrinter.hpp"

void writeGraphJSON(const Graph& graph, const std::string& filename) {
  GraphPrinter graphPrinter;
  std::ofstream json(filename);
  if (json.is_open()) {
    json << graphPrinter.graphToJSON(graph);
    json.close();
    std::cout << "Graph is written to " << filename << std::endl;
  } else {
    throw std::runtime_error("Unable to open file");
  }
}

int main() {
  const int inputDepth = 4;
  const int newVerticesNum = 3;
  const GraphGenerator::Params params(inputDepth, newVerticesNum);
  const GraphGenerator generator(params);
  const Graph& graph = generator.generate();
  writeGraphJSON(graph, "graph.json");
  return 0;
}
