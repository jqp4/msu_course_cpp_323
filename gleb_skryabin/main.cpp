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
  int inputDepth;
  int newVerticesNum;

  // std::cout << "Enter depth:\n";
  // std::cin >> inputDepth;
  // while (inputDepth < 0) {
  //   throw std::runtime_error("Invalid input depth");
  // }

  // std::cout << "Enter number of new vertices:\n";
  // std::cin >> newVerticesNum;
  // while (inputDepth < 0) {
  //   throw std::runtime_error("Invalid input newVerticesNum");
  // }

  inputDepth = 7;
  newVerticesNum = 3;

  const GraphGenerator::Params params(inputDepth, newVerticesNum);
  const GraphGenerator generator(params);
  const Graph& graph = generator.generate();
  writeGraphJSON(graph, "graph.json");
  return 0;
}
