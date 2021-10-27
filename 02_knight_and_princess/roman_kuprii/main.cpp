#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::endl;
using std::min;
using std::to_string;
using std::vector;

using EdgeId = int;
using VertexId = int;

constexpr int INVALID_ID = -1;
constexpr int INVALID_NEW_DEPTH = -1;
constexpr int INVALID_NEW_VERTICES_NUM = -1;
const std::string JSON_GRAPH_FILENAME = "graph.json";

enum class Color { Gray, Green, Blue, Yellow, Red };

std::string color_to_string(const Color& color) {
  std::string res = "";
  switch (color) {
    case Color::Gray: {
      res += "\"gray\" }";
      break;
    }
    case Color::Green: {
      res += "\"green\" }";
      break;
    }
    case Color::Blue: {
      res += "\"blue\" }";
      break;
    }
    case Color::Yellow: {
      res += "\"yellow\" }";
      break;
    }
    case Color::Red: {
      res += "\"red\" }";
      break;
    }
    default:
      break;
  }
  return res;
}

struct Edge {
  const EdgeId id = INVALID_ID;
  const std::array<VertexId, 2> connected_vertices;
  const Color color = Color::Gray;

  Edge(const VertexId& start,
       const VertexId& end,
       const EdgeId& _id,
       const Color& _color)
      : id(_id), connected_vertices({start, end}), color(_color) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id);
    res += ", \"vertex_ids\": [";
    res += to_string(connected_vertices[0]);
    res += ", ";
    res += to_string(connected_vertices[1]);
    res += "], \"color\": ";
    res += color_to_string(color);
    return res;
  }
};

bool is_edge_id_included(const EdgeId& id, const vector<EdgeId>& edge_ids) {
  for (const auto& edge_id : edge_ids)
    if (id == edge_id)
      return true;
  return false;
}

struct Vertex {
 public:
  const VertexId id = INVALID_ID;
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id) + ", \"edge_ids\": [";
    for (const auto& edge_id : edges_ids_) {
      res += to_string(edge_id);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "] }";
    return res;
  }

  void add_edge_id(const EdgeId& _id) {
    assert(!is_edge_id_included(_id, edges_ids_));
    edges_ids_.push_back(_id);
  }

  const vector<EdgeId>& get_edges_ids() const { return edges_ids_; }

 private:
  vector<EdgeId> edges_ids_;
};

class Graph {
 public:
  std::string to_json() const {
    std::string res;
    res = "{ \"depth\": ";
    res += to_string(depth_);
    res += ", \"vertices\": [ ";
    for (const auto& vertex : vertices_) {
      res += vertex.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ], \"edges\": [ ";
    for (const auto& edge : edges_) {
      res += edge.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ] }\n";
    return res;
  }

  void add_vertex() { vertices_.emplace_back(get_next_vertex_id()); }

  bool is_vertex_exist(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) {
      if (vertex_id == vertex.id)
        return true;
    }
    return false;
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const {
    assert(is_vertex_exist(from_vertex_id));
    assert(is_vertex_exist(to_vertex_id));

    const auto& from_vertex_edges_ids =
        vertices_[from_vertex_id].get_edges_ids();
    const auto& to_vertex_edges_ids = vertices_[to_vertex_id].get_edges_ids();
    for (const auto& from_vertex_edge_id : from_vertex_edges_ids)
      if (from_vertex_id == to_vertex_id) {
        const auto& connected_vertices =
            edges_[from_vertex_edge_id].connected_vertices;
        if (connected_vertices[0] == connected_vertices[1])
          return true;
      } else
        for (const auto& to_vertex_edge_id : to_vertex_edges_ids)
          if (from_vertex_edge_id == to_vertex_edge_id)
            return true;

    return false;
  }

  void connect_vertices(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id,
                        const bool& paint) {
    assert(is_vertex_exist(from_vertex_id));
    assert(is_vertex_exist(to_vertex_id));
    assert(!is_connected(from_vertex_id, to_vertex_id));

    if (!paint) {
      int min_depth = vertices_[from_vertex_id].depth;
      for (const auto& edge_idx : vertices_[to_vertex_id].get_edges_ids()) {
        VertexId vert = edges_[edge_idx].connected_vertices[0];
        min_depth = min(min_depth, vertices_[vert].depth);
      }
      vertices_[to_vertex_id].depth = min_depth + 1;

      if (depth_ < min_depth + 1)
        depth_ = min_depth + 1;
    }

    const int diff =
        vertices_[to_vertex_id].depth - vertices_[from_vertex_id].depth;

    Color color;
    if (!paint) {
      color = Color::Gray;
    } else if (from_vertex_id == to_vertex_id) {
      color = Color::Green;
    } else if (diff == 0) {
      color = Color::Blue;
    } else if (diff == 1) {
      color = Color::Yellow;
    } else if (diff == 2) {
      color = Color::Red;
    }

    const auto& new_edge = edges_.emplace_back(from_vertex_id, to_vertex_id,
                                               get_next_edge_id(), color);

    vertices_[from_vertex_id].add_edge_id(new_edge.id);
    if (from_vertex_id != to_vertex_id)
      vertices_[to_vertex_id].add_edge_id(new_edge.id);
  }

  vector<Edge> get_edges() const { return edges_; }
  vector<Vertex> get_vertices() const { return vertices_; }

  int get_depth() const { return depth_; }
  int get_vertices_num() const { return vertices_.size(); }

 private:
  vector<Vertex> vertices_;
  vector<Edge> edges_;
  int depth_ = 0;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_next_vertex_id() { return vertex_id_counter_++; }
  VertexId get_next_edge_id() { return edge_id_counter_++; }
};

void write_graph(const Graph& graph) {
  std::ofstream out;
  out.open(JSON_GRAPH_FILENAME, std::ofstream::out | std::ofstream::trunc);
  out << graph.to_json();
  out.close();
}

void new_vertices_generation(Graph& work_graph,
                             const int& depth,
                             const int& new_vertices_num) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  for (int current_depth = 0; current_depth <= depth; current_depth++) {
    const double probability =
        static_cast<double>(current_depth) / static_cast<double>(depth);
    for (const auto& vertex : work_graph.get_vertices())
      if (vertex.depth == current_depth)
        for (int iter = 0; iter < new_vertices_num; iter++) {
          if (dis(gen) > probability) {
            work_graph.add_vertex();
            work_graph.connect_vertices(
                vertex.id,
                work_graph.get_vertices()[work_graph.get_vertices_num() - 1].id,
                false);
          }
        }
  }
}

void paint_blue(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  for (int current_depth = 1; current_depth <= graph_depth; current_depth++) {
    vector<Vertex> uni_depth_vertices;
    for (const auto& vertex : work_graph.get_vertices())
      if (vertex.depth == current_depth)
        uni_depth_vertices.emplace_back(vertex);

    std::array<VertexId, 2> adjacent_vertices = {INVALID_ID, INVALID_ID};
    for (const auto& vertex : uni_depth_vertices) {
      if (adjacent_vertices[0] == INVALID_ID) {
        adjacent_vertices[0] = vertex.id;
      } else if (adjacent_vertices[1] == INVALID_ID) {
        adjacent_vertices[1] = vertex.id;
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (dis(gen) < 0.25)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], true);
      } else {
        adjacent_vertices[0] = adjacent_vertices[1];
        adjacent_vertices[1] = vertex.id;
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (dis(gen) < 0.25)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], true);
      }
    }
  }
}

void paint_green(Graph& work_graph) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  for (const auto& start_vertex : work_graph.get_vertices())
    if (!work_graph.is_connected(start_vertex.id, start_vertex.id))
      if (dis(gen) < 0.1)
        work_graph.connect_vertices(start_vertex.id, start_vertex.id, true);
}

void paint_red(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  for (const auto& start_vertex : work_graph.get_vertices()) {
    if (dis(gen) < 0.33) {
      if (start_vertex.depth + 2 <= graph_depth) {
        vector<VertexId> Red_vertices_ids;
        for (const auto& end_vertex : work_graph.get_vertices()) {
          if (end_vertex.depth == start_vertex.depth + 2)
            if (!work_graph.is_connected(start_vertex.id, end_vertex.id))
              Red_vertices_ids.emplace_back(end_vertex.id);
        }
        if (Red_vertices_ids.size() > 0) {
          std::uniform_int_distribution<> distr(0, Red_vertices_ids.size() - 1);
          work_graph.connect_vertices(start_vertex.id,
                                      Red_vertices_ids[distr(gen)], true);
        }
      }
    }
  }
}

void paint_yellow(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  for (const auto& start_vertex : work_graph.get_vertices()) {
    const double probability = static_cast<double>(start_vertex.depth) /
                               static_cast<double>(graph_depth);
    if (dis(gen) < probability) {
      vector<VertexId> Yellow_vertices_ids;
      for (const auto& end_vertex : work_graph.get_vertices()) {
        if (end_vertex.depth == start_vertex.depth + 1) {
          if (!work_graph.is_connected(start_vertex.id, end_vertex.id))
            Yellow_vertices_ids.push_back(end_vertex.id);
        }
      }
      if (Yellow_vertices_ids.size() > 0) {
        std::uniform_int_distribution<> distr(0,
                                              Yellow_vertices_ids.size() - 1);
        work_graph.connect_vertices(start_vertex.id,
                                    Yellow_vertices_ids[distr(gen)], true);
      }
    }
  }
}

void paint_edges(Graph& work_graph) {
  paint_blue(work_graph);
  paint_green(work_graph);
  paint_red(work_graph);
  paint_yellow(work_graph);
}

int main() {
  int depth = INVALID_NEW_DEPTH;
  do {
    std::cout << "Enter generate graph depth" << endl;
    std::cin >> depth;
  } while (depth < 0);
  int new_vertices_num = INVALID_NEW_VERTICES_NUM;
  do {
    std::cout << "Enter new_vertices_num" << endl;
    std::cin >> new_vertices_num;
  } while (new_vertices_num < 0);

  std::cout << "Depth of adding vertices: " << depth << endl;

  Graph my_graph;
  my_graph.add_vertex();
  new_vertices_generation(my_graph, depth, new_vertices_num);
  paint_edges(my_graph);
  write_graph(my_graph);

  return 0;
}
