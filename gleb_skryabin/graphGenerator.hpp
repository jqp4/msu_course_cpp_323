#include <assert.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "graph.hpp"

class GraphGenerator {
 public:
  struct Params {
   public:
    explicit Params(Depth depth = 0, int newVerticesCount = 0)
        : depth_(depth), newVerticesCount_(newVerticesCount) {}

    Depth depth() const { return depth_; }
    int newVerticesCount() const { return newVerticesCount_; }

   private:
    Depth depth_ = 0;
    int newVerticesCount_ = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() {
    std::srand(std::time(nullptr));
    Graph graph = Graph();
    const VertexId vertexSrcId = graph.addVertex();

    generateGrayEdges(graph, vertexSrcId);
    generateGreenEdges(graph);
    setVertexIdsByDepth(graph);
    generateBlueEdges(graph);
    generateYellowEdges(graph);

    return graph;
  }

 private:
  bool itHappened(float probability) {
    return probability >= float(rand() % 100) / 100;
  }

  float getColorProbability(const EdgeColor& color) const {
    switch (color) {
      case EDGE_COLOR_GREEN:
        return 0.1;
      case EDGE_COLOR_BLUE:
        return 0.25;
      case EDGE_COLOR_RED:
        return 0.33;
      default:
        throw std::runtime_error("Invalid edge color id");
        return 0.0;
    }
  }

  void setVertexIdsByDepth(Graph graph) {
    for (Depth depth = 1; depth < params_.depth(); depth++) {
      vertexIdsByDepth_.emplace(depth, graph.getVertexIdsByDepth(depth));
    }
  }

  void generateGrayEdges(Graph graph, VertexId vertexSrcId) {
    generateGrayBranches(graph, vertexSrcId, params_.depth() - 1);
  }

  /// Рекурсивная функция генерации ветки графа, только серые ребра (в начале
  /// граф генерируется как дерево глубиной не больше params_.depth())
  void generateGrayBranches(Graph graph,
                            const VertexId& vertexSrcId,
                            const Depth& nowDepth) {
    if (nowDepth > 0) {
      const float prob = float(nowDepth) / params_.depth();
      for (int j = 0; j < params_.newVerticesCount(); j++) {
        if (itHappened(prob)) {
          const VertexId vertexNewId =
              graph.addVertex(params_.depth() - nowDepth);
          graph.addEdge(vertexSrcId, vertexNewId, EDGE_COLOR_GRAY);
          generateGrayBranches(graph, vertexNewId, nowDepth - 1);
        }
      }
    }
  }

  void generateGreenEdges(Graph graph) {
    const float prob = getColorProbability(EDGE_COLOR_GREEN);
    for (const auto& [vertexId, vertex] : graph.getVertices()) {
      if (itHappened(prob)) {
        graph.addEdge(vertexId, vertexId, EDGE_COLOR_GREEN);
      }
    }
  }

  void generateBlueEdges(Graph graph) {
    const float prob = getColorProbability(EDGE_COLOR_BLUE);
    for (Depth depth = 1; depth < params_.depth(); depth++) {
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
            graph.addEdge(vertexSrcId, nearestVertexTrgId, EDGE_COLOR_BLUE);
          }
        }
      }
    }
  }

  void generateYellowEdges(Graph graph) {
    // const float prob = getColorProbability(EDGE_COLOR_YELLOW);
  }

  
  const Params params_ = Params();
  std::unordered_map<Depth, std::vector<VertexId>> vertexIdsByDepth_;
};
