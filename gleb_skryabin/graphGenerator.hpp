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
    setVertexIdsByDepth();
    generateBlueEdges();
    generateYellowEdges();
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

  void setVertexIdsByDepth() {
    for (Depth depth = 1; depth < graphDepth_; depth++) {
      vertexIdsByDepth_.emplace(depth, graph_.getVertexIdsByDepth(depth));
    }
  }

  void generateGrayEdges() {
    const VertexId vertexSrcId = graph_.addVertex();
    generateGrayBranches(vertexSrcId, graphDepth_ - 1);
  }

  /// Рекурсивная функция генерации ветки графа, только серые ребра (в начале
  /// граф генерируется как дерево глубиной не больше graphDepth_)
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
    for (const auto& [vertexId, vertex] : graph_.getVertices()) {
      if (itHappened(prob)) {
        graph_.addEdge(vertexId, vertexId, EDGE_COLOR_GREEN);
      }
    }
  }

  void generateBlueEdges() {
    float prob = getColorProbability(EDGE_COLOR_BLUE);
    for (Depth depth = 1; depth < graphDepth_; depth++) {
      const std::vector<VertexId> vertexIds = vertexIdsByDepth_.at(depth);
      if (vertexIds.size() > 1) {
        VertexId maxVertexId =
            *std::max_element(vertexIds.begin(), vertexIds.end());
        for (const VertexId& vertexSrcId : vertexIds) {
          if (vertexSrcId != maxVertexId && itHappened(prob)) {
            // Поиск соседней вершины для соединения
            VertexId nearestVertexTrgId = maxVertexId;
            for (const VertexId& vertexTrgId : vertexIds) {
              if (vertexTrgId > vertexSrcId &&
                  vertexTrgId < nearestVertexTrgId) {
                nearestVertexTrgId = vertexTrgId;
              }
            }
            graph_.addEdge(vertexSrcId, nearestVertexTrgId, EDGE_COLOR_BLUE);
          }
        }
      }
    }
  }

  void generateYellowEdges() {
    float prob = getColorProbability(EDGE_COLOR_YELLOW);
    for (Depth depth = 1; depth < graphDepth_; depth++) {
      const std::vector<VertexId> vertexIds = vertexIdsByDepth_.at(depth);
      if (vertexIds.size() > 1) {
        VertexId maxVertexId =
            *std::max_element(vertexIds.begin(), vertexIds.end());
        for (const VertexId& vertexSrcId : vertexIds) {
          if (vertexSrcId != maxVertexId && itHappened(prob)) {
            // Поиск соседней вершины для соединения
            VertexId nearestVertexTrgId = maxVertexId;
            for (const VertexId& vertexTrgId : vertexIds) {
              if (vertexTrgId > vertexSrcId &&
                  vertexTrgId < nearestVertexTrgId) {
                nearestVertexTrgId = vertexTrgId;
              }
            }
            graph_.addEdge(vertexSrcId, nearestVertexTrgId, EDGE_COLOR_YELLOW);
          }
        }
      }
    }
  }

  Graph graph_;
  const Depth graphDepth_ = 0;
  const int newVerticesNum_ = 0;
  std::unordered_map<Depth, std::vector<VertexId>> vertexIdsByDepth_;
};
