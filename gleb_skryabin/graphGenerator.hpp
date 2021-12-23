#include <assert.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>
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

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {
    std::srand(std::time(nullptr));
  }

  Graph generate() {
    Graph graph = Graph();
    const VertexId vertexSrcId = graph.addVertex();
    std::unordered_map<Depth, std::vector<VertexId>> vertexIdsByDepth;

    generateGrayEdges(graph, vertexSrcId);
    getVertexIdsByDepth(graph, vertexIdsByDepth);
    generateGreenEdges(graph);
    // generateBlueEdges(graph, vertexIdsByDepth);
    // generateYellowEdges(graph, vertexIdsByDepth);
    generateRedEdges(graph, vertexIdsByDepth);

    return graph;
  }

 private:
  bool itHappened(float probability) const {
    return probability >= float(rand() % 100) / 100;
  }

  float getColorProbability(const Edge::Colors& color,
                            const Depth& depth = 0) const {
    switch (color) {
      case Edge::Colors::Grey:
        return float(depth) / params_.depth();
      case Edge::Colors::Green:
        return 0.1;
      case Edge::Colors::Yellow:
        return 1.0 - float(depth) / params_.depth();
      case Edge::Colors::Red:
        return 0.33;
      default:
        throw std::runtime_error("Invalid Edge::Colors value");
        return 0.0;
    }
  }

  void getVertexIdsByDepth(Graph graph,
                           std::unordered_map<Depth, std::vector<VertexId>>&
                               vertexIdsByDepth) const {
    for (Depth depth = 0; depth < params_.depth(); depth++) {
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
    generateGrayBranches(graph, vertexSrcId, params_.depth() - 1);
  }

  /// Рекурсивная функция генерации ветки графа, только серые ребра (в начале
  /// граф генерируется как дерево глубиной не больше params_.depth())
  void generateGrayBranches(Graph& graph,
                            const VertexId& vertexSrcId,
                            const Depth& nowDepth) const {
    if (nowDepth > 0) {
      const float prob = getColorProbability(Edge::Colors::Grey, nowDepth);
      for (int j = 0; j < params_.newVerticesCount(); j++) {
        if (itHappened(prob)) {
          const VertexId vertexNewId =
              graph.addVertex(params_.depth() - nowDepth);
          graph.addEdge(vertexSrcId, vertexNewId, Edge::Colors::Grey);
          generateGrayBranches(graph, vertexNewId, nowDepth - 1);
        }
      }
    }
  }

  void generateGreenEdges(Graph& graph) {
    const float prob = getColorProbability(Edge::Colors::Green);
    for (const auto& [vertexId, vertex] : graph.getVertices()) {
      if (itHappened(prob)) {
        graph.addEdge(vertexId, vertexId, Edge::Colors::Green);
      }
    }
  }

  // void generateBlueEdges(
  //     Graph graph,
  //     std::unordered_map<Depth, std::vector<VertexId>> vertexIdsByDepth) {
  //   const float prob = getColorProbability(EDGE_COLOR_BLUE);
  //   for (Depth depth = 1; depth < params_.depth(); depth++) {
  //     const std::vector<VertexId> vertexIds = vertexIdsByDepth.at(depth);
  //     if (vertexIds.size() > 1) {
  //       VertexId maxVertexId =
  //           *std::max_element(vertexIds.begin(), vertexIds.end());
  //       for (const VertexId& vertexSrcId : vertexIds) {
  //         if (vertexSrcId != maxVertexId && itHappened(prob)) {
  //           // Поиск соседней вершины для соединения
  //           VertexId nearestVertexTrgId = maxVertexId;
  //           for (const VertexId& vertexTrgId : vertexIds) {
  //             if (vertexTrgId > vertexSrcId &&
  //                 vertexTrgId < nearestVertexTrgId) {
  //               nearestVertexTrgId = vertexTrgId;
  //             }
  //           }
  //           graph.addEdge(vertexSrcId, nearestVertexTrgId, EDGE_COLOR_BLUE);
  //         }
  //       }
  //     }
  //   }
  // }
  //
  // void generateYellowEdges(
  //     Graph graph,
  //     std::unordered_map<Depth, std::vector<VertexId>> vertexIdsByDepth) {
  //   // const float prob = getColorProbability(EDGE_COLOR_YELLOW);
  // }

  void generateRedEdges(
      Graph graph,
      std::unordered_map<Depth, std::vector<VertexId>> vertexIdsByDepth) {
    const float prob = getColorProbability(Edge::Colors::Red);
    for (Depth depth = params_.depth() - 1; depth > 1; depth--) {
      // std::cout << "depth " << depth << ": ";
      // for (const auto& vi : vertexIdsByDepth.at(depth)) {
      //   std::cout << vi << " ";
      // }
      // std::cout << std::endl;

      // std::cout << "depth " << depth << " --> " << depth - 2 << ": \n";
      for (const VertexId& vertexSrcId : vertexIdsByDepth.at(depth)) {
        // for (const VertexId& vertexTrgId : vertexIdsByDepth.at(depth - 2)) {
        //   if (itHappened(prob)) {
        //     graph.addEdge(vertexSrcId, vertexTrgId, Edge::Colors::Red);
        //     // std::cout << "  " << vertexSrcId << " --> " << vertexTrgId
        //     //           << std::endl;
        //   }
        // }
        if (itHappened(prob)) {
          const auto trgVertexIds = vertexIdsByDepth.at(depth - 2);
          // vertexTrgId = trgVertexIds.at(trgVertexIds.size())

          // graph.addEdge(vertexSrcId, vertexTrgId, Edge::Colors::Red);
        }
      }
    }
  }

  const Params params_ = Params();
};
