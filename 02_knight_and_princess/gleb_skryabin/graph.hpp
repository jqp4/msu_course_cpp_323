#include <assert.h>
#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using EdgeId = int;
using VertexId = int;
constexpr int INVALID_ID = -1;
constexpr int INVALID_DEPTH = -1;

enum {
  EDGE_COLOR_GRAY = 0,
  EDGE_COLOR_GREEN = 1,
  EDGE_COLOR_BLUE = 2,
  EDGE_COLOR_YELLOW = 3,
  EDGE_COLOR_RED = 4
};

std::string getEdgeColorStr(int colorId) {
  switch (colorId) {
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

class Edge {
 public:
  Edge(const EdgeId& inpId,
       const VertexId& vertexSrcId,
       const VertexId& vertexTrgId)
      : id_(inpId), vertexIds_(vertexSrcId, vertexTrgId) {}

  const std::pair<VertexId, VertexId>& getVertexIds() const {
    return vertexIds_;
  }

  std::string toJSON() const {
    std::string json;
    json += "{\n\"id\": " + std::to_string(id_);
    json += ",\n\"vertex_ids\": [";
    json += std::to_string(vertexIds_.first) + ", ";
    json += std::to_string(vertexIds_.second) + "],\n";
    json += "\"color\": \"" + getEdgeColorStr(color_);
    json += "\"\n}";
    return json;
  }

 private:
  const EdgeId id_ = INVALID_ID;
  const int color_ = EDGE_COLOR_GRAY;
  const std::pair<VertexId, VertexId> vertexIds_ = {INVALID_ID, INVALID_ID};
};

class Vertex {
 public:
  explicit Vertex(const VertexId& inpId) : id_(inpId) {}

  void addEdgeId(const EdgeId& edgeId) {
    assert(!containEdge(edgeId) && "ERROR: vertex already contain edge");
    edgeIds_.insert(edgeId);
  }

  bool containEdge(const EdgeId& edgeId) {
    return edgeIds_.find(edgeId) != edgeIds_.end();
  }

  const std::unordered_set<EdgeId>& getEdgeIds() const { return edgeIds_; }

  std::string toJSON() const {
    std::string json;
    json += "{\n\"id\": " + std::to_string(id_);
    json += ",\n\"edge_ids\": [";

    for (auto pEdgeId = edgeIds_.begin(); pEdgeId != edgeIds_.end();
         pEdgeId++) {
      json += pEdgeId != edgeIds_.begin() ? ", " : "";
      json += std::to_string(*pEdgeId);
    }

    json += "],\n\"depth\": ";
    json += std::to_string(depth_) + "\n}";
    return json;
  }

 private:
  const EdgeId id_ = INVALID_ID;
  const int depth_ = INVALID_DEPTH;
  std::unordered_set<EdgeId> edgeIds_ = {};
};

class Graph {
 public:
  void generate(int depth, int new_vertices_num) {
    float step = 100.0 / depth;
    float percent = 100.0;

    for (int i = 0; i < depth; i++) {
      std::cout << percent << std::endl;
      percent -= step;
    }
    percent = 0.0;
    std::cout << percent << std::endl;
  }

  EdgeId addEdge(const VertexId& vertexSrcId, const VertexId& vertexTrgId) {
    assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    assert(containVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
    assert(!checkConnectoin(vertexSrcId, vertexTrgId) &&
           "ERROR: edge doesn't exists");

    const EdgeId newEdgeId = generateEdgeId();
    edges_.emplace(newEdgeId, Edge(newEdgeId, vertexSrcId, vertexTrgId));
    vertices_.at(vertexSrcId).addEdgeId(newEdgeId);
    vertices_.at(vertexTrgId).addEdgeId(newEdgeId);
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

  VertexId addVertex() {
    const VertexId newVertexId = generateVertexId();
    vertices_.emplace(newVertexId, newVertexId);
    return newVertexId;
  }

  bool containVertex(const VertexId& vertexId) const {
    return vertices_.find(vertexId) != vertices_.end();
  }

  bool containEdge(const EdgeId& edgeId) const {
    return edges_.find(edgeId) != edges_.end();
  }

  std::string toJSON() const {
    std::string json;
    json = "{\n\"vertices\": [\n";
    for (auto pVertexPair = vertices_.begin(); pVertexPair != vertices_.end();
         pVertexPair++) {
      json += pVertexPair != vertices_.begin() ? ", " : "";
      json += pVertexPair->second.toJSON();
    }

    json += "\n ],\n\"edges\": [\n";
    for (auto pEdgePair = edges_.begin(); pEdgePair != edges_.end();
         pEdgePair++) {
      json += pEdgePair != edges_.begin() ? ", " : "";
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
