#include <assert.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
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
        return "-";
    }
  }

  std::string edgeToJSON(const Edge& edge) const {
    std::string json;
    const auto vertexIds = edge.getVertexIds();
    json += "{\"id\": " + std::to_string(edge.getId());
    json += ", \"vertex_ids\": [";
    json += std::to_string(vertexIds.first) + ", ";
    json += std::to_string(vertexIds.second) + "],";
    json += " \"color\": \"" + edgeColorToStr(edge.getColor());
    json += "\"}";
    return json;
  }

  std::string vertexToJSON(const Vertex& vertex) const {
    std::string json;
    const auto edgeIds = vertex.getEdgeIds();
    json += "{\"id\": " + std::to_string(vertex.getId());
    json += ", \"edge_ids\": [";

    for (auto edgeId = edgeIds.begin(); edgeId != edgeIds.end(); edgeId++) {
      json += edgeId != edgeIds.begin() ? ", " : "";
      json += std::to_string(*edgeId);
    }

    json += "], \"depth\": ";
    json += std::to_string(vertex.getDepth()) + "}";
    return json;
  }

  std::string graphToJSON(const Graph& graph) const {
    const auto vertices = graph.getVertices();
    const auto edges = graph.getEdges();

    std::string json;
    json = "{\n\"vertices\": [\n";
    for (auto vertexPair = vertices.begin(); vertexPair != vertices.end();
         vertexPair++) {
      json += vertexPair != vertices.begin() ? ",\n" : "";
      json += vertexToJSON(vertexPair->second);
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
