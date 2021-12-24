#include <assert.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#pragma once

using Depth = int;
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
  const Edge::Colors& getColor() const { return color_; }
  const std::pair<VertexId, VertexId>& getVertexIds() const {
    return vertexIds_;
  }

 private:
  const EdgeId id_ = INVALID_ID;
  const Edge::Colors color_ = Edge::Colors::Grey;
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
  EdgeId addEdge(const VertexId& vertexSrcId,
                 const VertexId& vertexTrgId,
                 const Edge::Colors& color = Edge::Colors::Grey) {
    const EdgeId newEdgeId = generateEdgeId();
    assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    if (vertexSrcId != vertexTrgId) {
      assert(containVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
      assert(!checkConnectoin(vertexSrcId, vertexTrgId) &&
             "ERROR: edge already exists");

      edges_.emplace(newEdgeId,
                     Edge(newEdgeId, vertexSrcId, vertexTrgId, color));
      connectivityList_.at(vertexSrcId).insert(newEdgeId);
      connectivityList_.at(vertexTrgId).insert(newEdgeId);
    } else {
      for (const auto& edgeId : connectivityList_.at(vertexSrcId)) {
        const auto& vs = edges_.at(edgeId).getVertexIds();
        assert(vs.first != vs.second && "ERROR: edge already exists");
      }

      edges_.emplace(newEdgeId,
                     Edge(newEdgeId, vertexSrcId, vertexSrcId, color));
      connectivityList_.at(vertexSrcId).insert(newEdgeId);
    }
    return newEdgeId;
  }

  bool checkConnectoin(const VertexId& vertexSrcId,
                       const VertexId& vertexTrgId) const {
    assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    assert(containVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
    const auto& srcEdgeIds = connectivityList_.at(vertexSrcId);
    const auto& trgEdgeIds = connectivityList_.at(vertexTrgId);

    for (const auto& srcEdgeId : srcEdgeIds) {
      for (const auto& trgEdgeId : trgEdgeIds) {
        if (srcEdgeId == trgEdgeId) {
          return true;
        }
      }
    }

    return false;
  }

  VertexId addVertex(const Depth& depth = DEFAULT_DEPTH) {
    const VertexId newVertexId = generateVertexId();
    connectivityList_.emplace(newVertexId, std::unordered_set<EdgeId>());
    const Vertex newVertex = Vertex(newVertexId);
    vertices_.emplace(newVertexId, newVertex);
    depths_.emplace(newVertexId, depth);
    return newVertexId;
  }

  bool containVertex(const VertexId& vertexId) const {
    return vertices_.find(vertexId) != vertices_.end();
  }

  bool containEdge(const EdgeId& edgeId) const {
    return edges_.find(edgeId) != edges_.end();
  }

  Depth getVertexDepth(const VertexId& vertexId) const {
    assert(containVertex(vertexId) && "ERROR: Vertex doesn't exists");
    return depths_.find(vertexId)->second;
  }

  const std::unordered_map<EdgeId, Edge>& getEdges() const { return edges_; }

  const std::unordered_map<VertexId, Vertex>& getVertices() const {
    return vertices_;
  }

  const std::unordered_map<VertexId, std::unordered_set<EdgeId>>&
  getConnectivityList() const {
    return connectivityList_;
  }

  std::vector<VertexId> getVertexIds() const {
    std::vector<VertexId> vertexIds(vertices_.size());
    auto selector = [](auto pair) { return pair.first; };
    transform(vertices_.begin(), vertices_.end(), vertexIds.begin(), selector);
    return vertexIds;
  }

 private:
  EdgeId nextEdgeId_ = 0;
  VertexId nextVertexId_ = 0;
  EdgeId generateEdgeId() { return nextEdgeId_++; }
  VertexId generateVertexId() { return nextVertexId_++; }

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<VertexId, std::unordered_set<EdgeId>> connectivityList_;
  std::unordered_map<VertexId, Depth> depths_;
};
