#include <assert.h>
#include <array>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include "graph.hpp"

constexpr float FULL_P_FLOAT = 100.0;
constexpr int FULL_P_INT = 100;

class GraphGenerator {
 public:
  GraphGenerator(const Depth& depth, int newVerticesNum)
      : graphDepth_(depth), newVerticesNum_(newVerticesNum) {
    std::srand(std::time(nullptr));
    graph_ = Graph();
  }

  void generateGraph() {
    const VertexId vertexSrcId = graph_.addVertex();
    generateBranch(vertexSrcId, graphDepth_ - 1);
  }

  const Graph& getGraph() const { return graph_; }

 private:
  /// Рекурсивная функция генерации ветки графа (в начале граф генерируется как
  /// дерево глубиной graphDepth_)
  void generateBranch(const VertexId& vertexSrcId, const Depth& nowDepth) {
    if (nowDepth > 0) {
      float p = FULL_P_FLOAT * nowDepth / graphDepth_;
      for (int j = 0; j < newVerticesNum_; j++) {
        if (p >= rand() % FULL_P_INT) {
          const VertexId vertexNewId = graph_.addVertex(graphDepth_ - nowDepth);
          graph_.addEdge(vertexSrcId, vertexNewId);
          generateBranch(vertexNewId, nowDepth - 1);
        }
      }
    }
  }

  Graph graph_;
  const Depth graphDepth_ = 0;
  const int newVerticesNum_ = 0;
};
