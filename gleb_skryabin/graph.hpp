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
  Vertex(const VertexId& inpId, const Depth& inpDepth)
      : id_(inpId), depth_(inpDepth) {}

  void addEdgeId(const EdgeId& edgeId) {
    assert(!containEdge(edgeId) && "ERROR: vertex already contain edge");
    edgeIds_.insert(edgeId);
  }

  bool containEdge(const EdgeId& edgeId) {
    return edgeIds_.find(edgeId) != edgeIds_.end();
  }

  const VertexId& getId() const { return id_; }
  const Depth& getDepth() const { return depth_; }
  const std::unordered_set<EdgeId>& getEdgeIds() const { return edgeIds_; }

 private:
  const VertexId id_ = INVALID_ID;
  const Depth depth_ = DEFAULT_DEPTH;
  std::unordered_set<EdgeId> edgeIds_ = {};
};

class Graph {
 public:
  EdgeId addEdge(const VertexId& vertexSrcId,
                 const VertexId& vertexTrgId,
                 const Edge::Colors& color = Edge::Colors::Grey) {
    const EdgeId newEdgeId = generateEdgeId();
    if (vertexSrcId != vertexTrgId) {
      assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
      assert(containVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
      assert(!checkConnectoin(vertexSrcId, vertexTrgId) &&
             "ERROR: edge already exists");

      edges_.emplace(newEdgeId,
                     Edge(newEdgeId, vertexSrcId, vertexTrgId, color));
      vertices_.at(vertexSrcId).addEdgeId(newEdgeId);
      vertices_.at(vertexTrgId).addEdgeId(newEdgeId);
    } else {
      assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
      for (const auto& edgeId : vertices_.at(vertexSrcId).getEdgeIds()) {
        const auto& vs = edges_.at(edgeId).getVertexIds();
        assert(vs.first != vs.second && "ERROR: edge already exists");
      }

      edges_.emplace(newEdgeId,
                     Edge(newEdgeId, vertexSrcId, vertexSrcId, color));
      vertices_.at(vertexSrcId).addEdgeId(newEdgeId);
    }
    return newEdgeId;
  }

  bool checkConnectoin(const VertexId& vertexSrcId,
                       const VertexId& vertexTrgId) const {
    assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    assert(containVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
    const auto& srcEdgeIds = vertices_.at(vertexSrcId).getEdgeIds();
    const auto& trgEdgeIds = vertices_.at(vertexTrgId).getEdgeIds();

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
    vertices_.emplace(newVertexId, Vertex(newVertexId, depth));
    return newVertexId;
  }

  bool containVertex(const VertexId& vertexId) const {
    return vertices_.find(vertexId) != vertices_.end();
  }

  bool containEdge(const EdgeId& edgeId) const {
    return edges_.find(edgeId) != edges_.end();
  }

  const std::unordered_map<EdgeId, Edge>& getEdges() const { return edges_; }

  const std::unordered_map<VertexId, Vertex>& getVertices() const {
    return vertices_;
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
};
