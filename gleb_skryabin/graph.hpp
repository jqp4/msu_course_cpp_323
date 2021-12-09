#include <assert.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Depth = int;
using EdgeId = int;
using VertexId = int;
using EdgeColor = int;
constexpr int INVALID_ID = -1;
constexpr int DEFAULT_DEPTH = 0;

enum {
  EDGE_COLOR_GRAY = 0,
  EDGE_COLOR_GREEN = 1,
  EDGE_COLOR_BLUE = 2,
  EDGE_COLOR_YELLOW = 3,
  EDGE_COLOR_RED = 4
};

class Edge {
 public:
  Edge(const EdgeId& inpId,
       const VertexId& vertexSrcId,
       const VertexId& vertexTrgId,
       const EdgeColor& color)
      : id_(inpId), vertexIds_(vertexSrcId, vertexTrgId), color_(color) {}

  const EdgeId& getId() const { return id_; }
  const Depth& getColor() const { return color_; }
  const std::pair<VertexId, VertexId>& getVertexIds() const {
    return vertexIds_;
  }

  std::string getColorStr() const {
    switch (color_) {
      case EDGE_COLOR_GRAY:
        return "gray";
      case EDGE_COLOR_GREEN:
        return "green";
      case EDGE_COLOR_BLUE:
        return "blue";
      case EDGE_COLOR_YELLOW:
        return "yellow";
      case EDGE_COLOR_RED:
        return "red";
      default:
        std::runtime_error("Invalid edge color id");
        return "-";
    }
  }

  std::string toJSON() const {
    std::string json;
    json += "{\"id\": " + std::to_string(id_);
    json += ",\"vertex_ids\": [";
    json += std::to_string(vertexIds_.first) + ", ";
    json += std::to_string(vertexIds_.second) + "],";
    json += "\"color\": \"" + getColorStr();
    json += "\"}";
    return json;
  }

 private:
  const EdgeId id_ = INVALID_ID;
  const EdgeColor color_ = EDGE_COLOR_GRAY;
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

  std::string toJSON() const {
    std::string json;
    json += "{\"id\": " + std::to_string(id_);
    json += ",\"edge_ids\": [";

    for (auto pEdgeId = edgeIds_.begin(); pEdgeId != edgeIds_.end();
         pEdgeId++) {
      json += pEdgeId != edgeIds_.begin() ? ", " : "";
      json += std::to_string(*pEdgeId);
    }

    json += "],\"depth\": ";
    json += std::to_string(depth_) + "}";
    return json;
  }

 private:
  const VertexId id_ = INVALID_ID;
  const Depth depth_ = DEFAULT_DEPTH;
  std::unordered_set<EdgeId> edgeIds_ = {};
};

class Graph {
 public:
  EdgeId addEdge(const VertexId& vertexSrcId,
                 const VertexId& vertexTrgId,
                 const EdgeColor& color = EDGE_COLOR_GRAY) {
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

  const std::unordered_map<VertexId, Vertex>& getVertices() {
    return vertices_;
  }

  std::vector<VertexId> getVertexIds() {
    std::vector<VertexId> vertexIds(vertices_.size());
    auto selector = [](auto pair) { return pair.first; };
    transform(vertices_.begin(), vertices_.end(), vertexIds.begin(), selector);
    return vertexIds;
  }

  std::vector<VertexId> getVertexIdsByDepth(const Depth& depth) {
    std::vector<VertexId> vertexIds;
    for (const auto& [vertexId, vertex] : vertices_) {
      if (vertex.getDepth() == depth) {
        vertexIds.push_back(vertexId);
      }
    }
    return vertexIds;
  }

  std::string toJSON() const {
    std::string json;
    json = "{\n\"vertices\": [\n";
    for (auto pVertexPair = vertices_.begin(); pVertexPair != vertices_.end();
         pVertexPair++) {
      json += pVertexPair != vertices_.begin() ? ",\n" : "";
      json += pVertexPair->second.toJSON();
    }

    json += "\n ],\n\"edges\": [\n";
    for (auto pEdgePair = edges_.begin(); pEdgePair != edges_.end();
         pEdgePair++) {
      json += pEdgePair != edges_.begin() ? ",\n" : "";
      json += pEdgePair->second.toJSON();
    }

    json += "\n]\n}\n";
    return json;
  }

 private:
  EdgeId nextEdgeId_ = 0;
  VertexId nextVertexId_ = 0;
  EdgeId generateEdgeId() { return nextEdgeId_++; }
  VertexId generateVertexId() { return nextVertexId_++; }

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
};
