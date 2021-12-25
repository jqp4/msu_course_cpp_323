#include <fstream>
#include <iostream>
#include "graphGenerator.hpp"
#include "graphPrinter.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
  // const GraphGenerator::Params params(inputDepth, newVerticesNum);
  GraphGenerator::Params params =
      GraphGenerator::Params(inputDepth, newVerticesNum);
  GraphGenerator generator = GraphGenerator(params);
  Graph graph = generator.generate();
  writeGraphJSON(graph, "graph.json");
  return 0;
}
