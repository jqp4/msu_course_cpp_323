#include <assert.h>
#include <array>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "graph.hpp"

class GraphGenerator {
 public:
  GraphGenerator(const Depth& depth, int newVerticesNum)
      : graphDepth_(depth), newVerticesNum_(newVerticesNum) {
    std::srand(std::time(nullptr));
    graph_ = Graph();
  }

  void generateGraph() {
    generateGrayEdges();
    generateGreenEdges();
  }

  const Graph& getGraph() const { return graph_; }

 private:
  bool itHappened(float probability) {
    return probability >= float(rand()) / RAND_MAX;
  }

  float getColorProbability(const EdgeColor& color) {
    switch (color) {
      case EDGE_COLOR_GREEN:
        return 0.1;
      case EDGE_COLOR_BLUE:
        return 0.25;
      case EDGE_COLOR_RED:
        return 0.33;
      default:
        std::runtime_error("Invalid edge color id");
        return 0.0;
    }
  }

  void generateGrayEdges() {
    const VertexId vertexSrcId = graph_.addVertex();
    generateGrayBranches(vertexSrcId, graphDepth_ - 1);
  }

  /// Рекурсивная функция генерации ветки графа, только серые ребра (в начале
  /// граф генерируется как дерево глубиной graphDepth_)
  void generateGrayBranches(const VertexId& vertexSrcId,
                            const Depth& nowDepth) {
    if (nowDepth > 0) {
      float prob = float(nowDepth) / graphDepth_;
      for (int j = 0; j < newVerticesNum_; j++) {
        if (itHappened(prob)) {
          const VertexId vertexNewId = graph_.addVertex(graphDepth_ - nowDepth);
          graph_.addEdge(vertexSrcId, vertexNewId, EDGE_COLOR_GRAY);
          generateGrayBranches(vertexNewId, nowDepth - 1);
        }
      }
    }
  }

  void generateGreenEdges() {
    float prob = getColorProbability(EDGE_COLOR_GREEN);
    for (const auto& vertexId : graph_.getVertexIds()) {
      if (itHappened(prob)) {
        graph_.addEdge(vertexId, vertexId, EDGE_COLOR_GREEN);
      }
    }
  }

  Graph graph_;
  const Depth graphDepth_ = 0;
  const int newVerticesNum_ = 0;
};
