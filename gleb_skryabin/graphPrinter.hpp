#include <unordered_set>
#include "graph.hpp"

class GraphPrinter {
 public:
  std::string edgeColorToStr(const Edge::Colors& color) const {
    switch (color) {
      case Edge::Colors::Grey:
        return "gray";
      case Edge::Colors::Green:
        return "green";
      case Edge::Colors::Yellow:
        return "yellow";
      case Edge::Colors::Red:
        return "red";
      default:
        throw std::runtime_error("Invalid Edge::Colors value");
    }
  }

  std::string edgeToJSON(const Edge& edge) const {
    std::string json;
    const auto& vertexIds = edge.getVertexIds();
    json += "{\"id\": " + std::to_string(edge.getId());
    json += ", \"vertex_ids\": [";
    json += std::to_string(vertexIds.first) + ", ";
    json += std::to_string(vertexIds.second) + "],";
    json += " \"color\": \"" + edgeColorToStr(edge.getColor());
    json += "\"}";
    return json;
  }

  std::string vertexToJSON(const VertexId& vertexId,
                           const std::unordered_set<EdgeId>& edgeIds,
                           const Graph::Depth& depth) const {
    std::string json;
    json += "{\"id\": " + std::to_string(vertexId);
    json += ", \"edge_ids\": [";

    for (auto edgeId = edgeIds.begin(); edgeId != edgeIds.end(); edgeId++) {
      json += edgeId != edgeIds.begin() ? ", " : "";
      json += std::to_string(*edgeId);
    }

    json += "], \"depth\": ";
    json += std::to_string(depth) + "}";
    return json;
  }

  std::string graphToJSON(const Graph& graph) const {
    const auto& connectivityList = graph.getConnectivityList();
    const auto& edges = graph.getEdges();

    std::string json;
    json = "{\n\"vertices\": [\n";
    for (auto vertexPair = connectivityList.begin();
         vertexPair != connectivityList.end(); vertexPair++) {
      json += vertexPair != connectivityList.begin() ? ",\n" : "";
      json += vertexToJSON(vertexPair->first, vertexPair->second,
                           graph.getVertexDepth(vertexPair->first));
    }

    json += "\n ],\n\"edges\": [\n";
    for (auto edgePair = edges.begin(); edgePair != edges.end(); edgePair++) {
      json += edgePair != edges.begin() ? ",\n" : "";
      json += edgeToJSON(edgePair->second);
    }

    json += "\n]\n}\n";
    return json;
  }
};
