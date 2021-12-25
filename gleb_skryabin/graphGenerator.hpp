#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "graph.hpp"

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth = 0, int newVerticesCount = 0)
        : depth_(depth), newVerticesCount_(newVerticesCount) {}

    const Graph::Depth getDepth() const { return depth_; }
    const int getNewVerticesCount() const { return newVerticesCount_; }

   private:
    const Graph::Depth depth_ = 0;
    const int newVerticesCount_ = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const {
    Graph graph = Graph();
    const VertexId& vertexSrcId = graph.addVertex();
    graph.setVertexDepth(vertexSrcId, 0);

    generateGrayEdges(graph, vertexSrcId);
    generateGreenEdges(graph);
    generateYellowEdges(graph);
    generateRedEdges(graph);

    return graph;
  }

 private:
  bool itHappened(float probability) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_number(0, 100);
    return probability >= float(random_number(gen)) / 100;
  }

  float getColorProbability(const Edge::Colors& color,
                            const Graph::Depth& depth = 0) const {
    switch (color) {
      case Edge::Colors::Grey:
        return float(depth) / params_.getDepth();
      case Edge::Colors::Green:
        return 0.1;
      case Edge::Colors::Yellow:
        return 1.0 - float(depth) / params_.getDepth();
      case Edge::Colors::Red:
        return 0.33;
      default:
        throw std::runtime_error("Invalid Edge::Colors value");
    }
  }

  void generateGrayEdges(Graph& graph, VertexId vertexSrcId) const {
    generateGrayBranches(graph, vertexSrcId, params_.getDepth() - 1);
  }

  /// Рекурсивная функция генерации ветки графа, только серые ребра (в начале
  /// граф генерируется как дерево глубиной не больше params_.depth())
  void generateGrayBranches(Graph& graph,
                            const VertexId& vertexSrcId,
                            const Graph::Depth& nowDepth) const {
    if (nowDepth > 0) {
      const float prob = getColorProbability(Edge::Colors::Grey, nowDepth);
      for (int j = 0; j < params_.getNewVerticesCount(); j++) {
        if (itHappened(prob)) {
          const VertexId vertexNewId = graph.addVertex();
          graph.setVertexDepth(vertexNewId, params_.getDepth() - nowDepth);
          graph.addEdge(vertexSrcId, vertexNewId, Edge::Colors::Grey);
          generateGrayBranches(graph, vertexNewId, nowDepth - 1);
        }
      }
    }
  }

  void generateGreenEdges(Graph& graph) const {
    const float prob = getColorProbability(Edge::Colors::Green);
    for (const auto& [vertexId, vertex] : graph.getVertices()) {
      if (itHappened(prob)) {
        graph.addEdge(vertexId, vertexId, Edge::Colors::Green);
      }
    }
  }

  void generateYellowEdges(Graph& graph) const {
    for (Graph::Depth depth = 1; depth < graph.getDepth() - 1; depth++) {
      const float prob = getColorProbability(Edge::Colors::Yellow, depth);
      for (const VertexId& vertexSrcId : graph.getDepthMap().at(depth)) {
        if (itHappened(prob)) {
          const auto& VertexTrgIds = graph.getDepthMap().at(depth + 1);
          const VertexId vertexTrgId = getRandomVertexIdExceptUnconnected(
              graph, vertexSrcId, VertexTrgIds);
          if (vertexTrgId != INVALID_ID) {
            graph.addEdge(vertexSrcId, vertexTrgId, Edge::Colors::Yellow);
          }
        }
      }
    }
  }

  void generateRedEdges(Graph& graph) const {
    const float prob = getColorProbability(Edge::Colors::Red);
    for (Graph::Depth depth = graph.getDepth() - 1; depth > 1; depth--) {
      for (const VertexId& vertexSrcId : graph.getDepthMap().at(depth)) {
        if (itHappened(prob)) {
          const auto& VertexTrgIds = graph.getDepthMap().at(depth - 2);
          const VertexId vertexTrgId = getRandomVertexId(VertexTrgIds);
          graph.addEdge(vertexSrcId, vertexTrgId, Edge::Colors::Red);
        }
      }
    }
  }

  VertexId getRandomVertexId(const std::vector<VertexId>& vertexIds) const {
    if (vertexIds.size() > 0) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> random_number(0, vertexIds.size() - 1);
      return vertexIds.at(random_number(gen));
    } else {
      return INVALID_ID;
    }
  }

  VertexId getRandomVertexIdExceptUnconnected(
      const Graph& graph,
      const VertexId& ParentVertexId,
      std::vector<VertexId> vertexIds) const {
    for (size_t i = 0; i < vertexIds.size(); i++) {
      if (graph.checkConnectoin(ParentVertexId, vertexIds.at(i))) {
        vertexIds.erase(std::next(vertexIds.begin(), i--));
      }
    }

    return getRandomVertexId(vertexIds);
  }

  const Params params_ = Params();
};
