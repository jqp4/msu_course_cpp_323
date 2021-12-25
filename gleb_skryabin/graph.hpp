#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using EdgeId = int;
using VertexId = int;
constexpr int INVALID_ID = -1;
constexpr int DEFAULT_DEPTH = 0;

class Edge {
 public:
  enum class Colors { Grey, Green, Blue, Yellow, Red };

  Edge(const EdgeId& inpId,
       const VertexId& vertexSrcId,
       const VertexId& vertexTrgId,
       const Colors& color)
      : id_(inpId), vertexIds_(vertexSrcId, vertexTrgId), color_(color) {}

  const EdgeId& getId() const { return id_; }
  const Colors& getColor() const { return color_; }
  const std::pair<VertexId, VertexId>& getVertexIds() const {
    return vertexIds_;
  }

 private:
  const EdgeId id_ = INVALID_ID;
  const Colors color_ = Colors::Grey;
  const std::pair<VertexId, VertexId> vertexIds_ = {INVALID_ID, INVALID_ID};
};

class Vertex {
 public:
  explicit Vertex(const VertexId& id) : id_(id) {}
  const VertexId& getId() const { return id_; }

 private:
  const VertexId id_;
};

class Graph {
 public:
  using Depth = int;

  EdgeId addEdge(const VertexId& vertexSrcId,
                 const VertexId& vertexTrgId,
                 const Edge::Colors& color = Edge::Colors::Grey) {
    assert(isContainVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    assert(isContainVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
    assert(!checkConnectoin(vertexSrcId, vertexTrgId) &&
           "ERROR: edge already exists");

    const EdgeId newEdgeId = generateEdgeId();
    edges_.emplace(newEdgeId, Edge(newEdgeId, vertexSrcId, vertexTrgId, color));
    connectivityList_.at(vertexSrcId).insert(newEdgeId);
    connectivityList_.at(vertexTrgId).insert(newEdgeId);
    return newEdgeId;
  }

  bool checkConnectoin(const VertexId& vertexSrcId,
                       const VertexId& vertexTrgId) const {
    assert(isContainVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    assert(isContainVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
    if (vertexSrcId != vertexTrgId) {
      for (const auto& srcEdgeId : connectivityList_.at(vertexSrcId)) {
        for (const auto& trgEdgeId : connectivityList_.at(vertexTrgId)) {
          if (srcEdgeId == trgEdgeId) {
            return true;
          }
        }
      }
    } else {
      for (const auto& edgeId : connectivityList_.at(vertexSrcId)) {
        const auto& connnectedVertexIds = edges_.at(edgeId).getVertexIds();
        if (connnectedVertexIds.first == connnectedVertexIds.second) {
          return true;
        }
      }
    }

    return false;
  }

  VertexId addVertex() {
    const VertexId newVertexId = generateVertexId();
    connectivityList_.emplace(newVertexId, std::unordered_set<EdgeId>());
    const Vertex newVertex = Vertex(newVertexId);
    vertices_.emplace(newVertexId, newVertex);
    return newVertexId;
  }

  void setVertexDepth(const VertexId& vertexId, const Depth& depth) {
    verticesDepths_.emplace(vertexId, depth);

    if (depthMap_.size() > depth) {
      depthMap_.at(depth).push_back(vertexId);
    } else {
      const std::vector<VertexId> newLayer = {vertexId};
      depthMap_.push_back(newLayer);
    }
  }

  bool isContainVertex(const VertexId& vertexId) const {
    return vertices_.find(vertexId) != vertices_.end();
  }

  bool isContainEdge(const EdgeId& edgeId) const {
    return edges_.find(edgeId) != edges_.end();
  }

  Depth getVertexDepth(const VertexId& vertexId) const {
    assert(isContainVertex(vertexId) && "ERROR: Vertex doesn't exists");
    return verticesDepths_.at(vertexId);
  }

  const Depth getDepth() const { return depthMap_.size(); }

  const std::unordered_map<EdgeId, Edge>& getEdges() const { return edges_; }

  const std::unordered_map<VertexId, Vertex>& getVertices() const {
    return vertices_;
  }

  const std::unordered_map<VertexId, std::unordered_set<EdgeId>>&
  getConnectivityList() const {
    return connectivityList_;
  }

  const std::vector<std::vector<VertexId>>& getDepthMap() const {
    return depthMap_;
  }

 private:
  EdgeId nextEdgeId_ = 0;
  VertexId nextVertexId_ = 0;
  EdgeId generateEdgeId() { return nextEdgeId_++; }
  VertexId generateVertexId() { return nextVertexId_++; }

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<VertexId, std::unordered_set<EdgeId>> connectivityList_;
  std::unordered_map<VertexId, Depth> verticesDepths_;
  std::vector<std::vector<VertexId>> depthMap_;
};
