#include <assert.h>
#include <algorithm>
#include <array>
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
    explicit Params(Depth depth = 0, int newVerticesCount = 0)
        : depth_(depth), newVerticesCount_(newVerticesCount) {}

    Depth getDepth() const { return depth_; }
    int getNewVerticesCount() const { return newVerticesCount_; }

   private:
    Depth depth_ = 0;
    int newVerticesCount_ = 0;
  };

  using DepthToIds = std::unordered_map<Depth, std::vector<VertexId>>;

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const {
    Graph graph = Graph();
    const VertexId vertexSrcId = graph.addVertex();
    DepthToIds vertexIdsByDepth;

    generateGrayEdges(graph, vertexSrcId);
    getVertexIdsByDepth(graph, vertexIdsByDepth);
    generateGreenEdges(graph);
    generateYellowEdges(graph, vertexIdsByDepth);
    generateRedEdges(graph, vertexIdsByDepth);

    return graph;
  }

 private:
  bool itHappened(float probability) const {
    // return probability >= float(rand() % 100) / 100;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_number(0, 100);
    return probability >= float(random_number(gen)) / 100;
  }

  float getColorProbability(const Edge::Colors& color,
                            const Depth& depth = 0) const {
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
        return 0.0;
    }
  }

  void getVertexIdsByDepth(Graph graph, DepthToIds& vertexIdsByDepth) const {
    for (Depth depth = 0; depth < params_.getDepth(); depth++) {
      std::vector<VertexId> vertexIds;
      for (const auto& [vertexId, vertex] : graph.getVertices()) {
        if (vertex.getDepth() == depth) {
          vertexIds.push_back(vertexId);
        }
      }

      vertexIdsByDepth.emplace(depth, vertexIds);
    }
  }

  void generateGrayEdges(Graph& graph, VertexId vertexSrcId) const {
    generateGrayBranches(graph, vertexSrcId, params_.getDepth() - 1);
  }

  /// Рекурсивная функция генерации ветки графа, только серые ребра (в начале
  /// граф генерируется как дерево глубиной не больше params_.depth())
  void generateGrayBranches(Graph& graph,
                            const VertexId& vertexSrcId,
                            const Depth& nowDepth) const {
    if (nowDepth > 0) {
      const float prob = getColorProbability(Edge::Colors::Grey, nowDepth);
      for (int j = 0; j < params_.getNewVerticesCount(); j++) {
        if (itHappened(prob)) {
          const VertexId vertexNewId =
              graph.addVertex(params_.getDepth() - nowDepth);
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

  void generateYellowEdges(Graph& graph,
                           const DepthToIds& vertexIdsByDepth) const {
    for (Depth depth = 1; depth < params_.getDepth() - 1; depth++) {
      const float prob = getColorProbability(Edge::Colors::Yellow, depth);
      for (const VertexId& vertexSrcId : vertexIdsByDepth.at(depth)) {
        if (itHappened(prob)) {
          const auto VertexTrgIds = vertexIdsByDepth.at(depth + 1);
          const VertexId vertexTrgId =
              getRandomVertexIdExceptSons(graph, vertexSrcId, VertexTrgIds);
          graph.addEdge(vertexSrcId, vertexTrgId, Edge::Colors::Yellow);
        }
      }
    }
  }

  void generateRedEdges(Graph& graph,
                        const DepthToIds& vertexIdsByDepth) const {
    const float prob = getColorProbability(Edge::Colors::Red);
    for (Depth depth = params_.getDepth() - 1; depth > 1; depth--) {
      for (const VertexId& vertexSrcId : vertexIdsByDepth.at(depth)) {
        if (itHappened(prob)) {
          const auto VertexTrgIds = vertexIdsByDepth.at(depth - 2);
          const VertexId vertexTrgId = getRandomVertexId(VertexTrgIds);
          graph.addEdge(vertexSrcId, vertexTrgId, Edge::Colors::Red);
        }
      }
    }
  }

  int getRandomVertexId(const std::vector<VertexId>& vertexIds) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_number(0, vertexIds.size() - 1);
    return vertexIds.at(random_number(gen));
  }

  int getRandomVertexIdExceptSons(const Graph& graph,
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
