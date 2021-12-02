#include <assert.h>
#include <unordered_map>
#include <unordered_set>

using VertexId = int;
using EdgeId = int;
constexpr int INVALID_ID = -1;

class Vertex {
 public:
  explicit Vertex(const VertexId& _id) : id_(_id) {}

  void addEdgeId(const EdgeId& edgeId) {
    assert(!containEdge(edgeId) && "ERROR: vertex already contain edge");
    edgeIds_.insert(edgeId);
  }

  bool containEdge(const EdgeId& edgeId) {
    return edgeIds_.find(edgeId) != edgeIds_.end();
  }

  std::string toJSON() const {
    std::string json;

    json += "{\"Id\": " + std::to_string(id_);
    json += ",\"Edge_Ids\": [";

    for (auto pEdgeId = edgeIds_.begin(); pEdgeId != edgeIds_.end();
         pEdgeId++) {
      if (pEdgeId != edgeIds_.begin()) {
        json += ", ";
      }

      json += std::to_string(*pEdgeId);
    }

    json += "]}";

    return json;
  }

 private:
  std::unordered_set<EdgeId> edgeIds_ = {};
  const VertexId id_ = 0;
};

class Edge {
 public:
  Edge(const EdgeId& _id,
       const VertexId& _from_vertex_id,
       const VertexId& _to_vertex_id)
      : id_(_id),
        from_vertex_id_(_from_vertex_id),
        to_vertex_id_(_to_vertex_id){};

  std::string toJSON() const {
    std::string json;

    json += "{\"Id\": " + std::to_string(id_) + ",\"Vertex_Ids\": [" +
            std::to_string(from_vertex_id_) + ", " +
            std::to_string(to_vertex_id_) + "]}";

    return json;
  }

 private:
  const VertexId from_vertex_id_ = INVALID_ID;
  const VertexId to_vertex_id_ = INVALID_ID;
  const EdgeId id_ = INVALID_ID;
};

class Graph {
 public:
  VertexId add_vertex() {
    const VertexId newVertexId = generateVertexId();
    vertices_.emplace(newVertexId, newVertexId);

    return newVertexId;
  }

  EdgeId add_edge(const VertexId& from_vertex_id,
                  const VertexId& to_vertex_id) {
    const EdgeId newEdgeId = generateEdgeId();

    edges_.emplace(newEdgeId, Edge(newEdgeId, from_vertex_id, to_vertex_id));
    vertices_.at(from_vertex_id).addEdgeId(newEdgeId);
    vertices_.at(to_vertex_id).addEdgeId(newEdgeId);

    return newEdgeId;
  }

  std::string toJSON() const {
    std::string json;
    json = "{\"Vertices\": [";

    for (auto pVertexPair = vertices_.begin(); pVertexPair != vertices_.end();
         pVertexPair++) {
      if (pVertexPair != vertices_.begin()) {
        json += ", ";
      }

      json += pVertexPair->second.toJSON();
    }

    json += " ],\"Edges\": [";

    for (auto pEdgePair = edges_.begin(); pEdgePair != edges_.end();
         pEdgePair++) {
      if (pEdgePair != edges_.begin()) {
        json += ", ";
      }

      json += pEdgePair->second.toJSON();
    }

    json += "]}";

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