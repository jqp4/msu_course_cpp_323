#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& id) : id_(id) {}

 private:
  const VertexId id_ = 0;
};

class Edge {
 public:
  Edge(const EdgeId& id,
       const VertexId& _from_vertex_id,
       const VertexId& _to_vertex_id)
      : id_(id),
        from_vertex_id_(_from_vertex_id),
        to_vertex_id_(_to_vertex_id){};

 private:
  const VertexId from_vertex_id_ = 0;
  const VertexId to_vertex_id_ = 0;
  const EdgeId id_ = 0;
};

class Graph {
 public:
  VertexId add_vertex() {
    const VertexId newVertexId = generateVertexId();
    vertices_.emplace(newVertexId, newVertexId);
    std::unordered_set<EdgeId> emptySet;
    connectivityList_.emplace(newVertexId, emptySet);

    return newVertexId;
  }

  EdgeId add_edge(const VertexId& from_vertex_id,
                  const VertexId& to_vertex_id) {
    const EdgeId newEdgeId = generateEdgeId();

    edges_.emplace(newEdgeId, Edge(newEdgeId, from_vertex_id, to_vertex_id));
    connectivityList_.at(from_vertex_id).insert(newEdgeId);
    connectivityList_.at(to_vertex_id).insert(newEdgeId);

    return newEdgeId;
  }

  //функция для проверки правильности. удалю с началом 3го задания
  void debug() const {
    std::cout << "Connectivity List:\n";
    for (const auto& [vid, eidSet] : connectivityList_) {
      std::cout << vid << ": [";
      for (const auto& eid : eidSet) {
        std::cout << eid << ", ";
      }
      std::cout << "]\n";
    }
  }

 private:
  EdgeId nextEdgeId_ = 0;
  VertexId nextVertexId_ = 0;
  EdgeId generateEdgeId() { return nextEdgeId_++; }
  VertexId generateVertexId() { return nextVertexId_++; }

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<VertexId, std::unordered_set<EdgeId>> connectivityList_;
};
