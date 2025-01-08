// Dijkstra's algorithm is an algorithm for finding the shortest paths between
// nodes in a graph, which may represent, for example, road networks. In this
// Koan you will complete the implementation of a class that calculates the
// lowest cost path between two vertices in a graph using a particularly
// elegant solution to the problem using a priority queue.
//
// See
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Using_a_priority_queue
// for a description of the algorithm that you will need.
//
// Some notes:
// - The costs of the edges are always positive.
// - There is a priority_queue container in the standard library that you can
//   use, but it does not have the add_with_priority and decrease_priority
//   methods that are described in the Wikipedia article. Look for the version
//   of the algorithm that does not need these methods, it is mentioned in the
//   same article.
// - You shortest_path() method must return both the sequence of nodes and the
//   sequence of edges that make up the path. The nodes and edges must be in
//   the correct order.

#include <algorithm>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "gtest/gtest.h"

/// BEGIN EDIT ------------------------------------------------------

//
// This block may or may not be necessary depending on your implementation.
// Feel free to use it if you need it, or delete it if you don't.
// I selected to do it on the shortesPathCalculator class
//

/// END EDIT --------------------------------------------------------

class ShortestPathCalculator {
public:
  using id_type = std::size_t;    // Alias for node/edge IDs.
  using cost_type = std::size_t; // Alias for edge costs.

  // Adds a vertex and returns its unique ID.
  id_type add_vertex() {
    auto id = make_id(); // Generate it
    graph_.emplace(id, std::vector<ConnectionListItem>{}); // Create an entry on the adjacency matrix
    return id;
  }

  // Adds a directed edge with a cost and returns its unique ID.
  // Throws if either vertex doesn't exist.
  id_type add_edge(id_type from, id_type to, cost_type edge_cost) {
    if (graph_.find(from) == graph_.end() || graph_.find(to) == graph_.end()) {
      throw std::runtime_error("Invalid vertex"); // just in case
    }
    // Gen id
    
    auto edge_id = make_id();
    
    // Add the edge id

    graph_[from].emplace_back(ConnectionListItem{to, edge_id, edge_cost});
    return edge_id;
  }

  // Finds the shortest path from source to destination using Dijkstra's algorithm.
  // I used this source: https://www.youtube.com/watch?v=bZkzH5x0SKU&ab_channel=FelixTechTips (great video)
  // Returns a tuple of nodes and edges in the path to estimate the matrix
  auto shortest_path(id_type src_node_id, id_type dest_node_id) const {
    using PriorityQueueItem = std::pair<cost_type, id_type>; // Priority queue item: {cost, vertex ID}.
    std::priority_queue<PriorityQueueItem, std::vector<PriorityQueueItem>, std::greater<>> pq;
    
    // Establish that the distance between the initial and the unknown is infinity
    std::unordered_map<id_type, cost_type> distances;
    std::unordered_map<id_type, id_type> predecessors, edge_used;
    std::set<id_type> visited;
    
    // Set the source node distance as 0 and other still not visited as infinity 
    for (const auto& [node_id, _] : graph_) distances[node_id] = std::numeric_limits<cost_type>::max();
    distances[src_node_id] = 0;
    pq.emplace(0, src_node_id);
    
    // Process the nodes 
    while (!pq.empty()) {
      // Get the node with the smallest distance from the priority queue 
      auto [current_cost, current_node] = pq.top();
      pq.pop();
      // Skip visited
      if (visited.count(current_node)) continue;
      visited.insert(current_node);
      // Break in case to fullfill
      if (current_node == dest_node_id) break;

      for (const auto& edge : graph_.at(current_node)) {
        if (visited.count(edge.dst_vertex_id)) continue;
        cost_type new_cost = current_cost + edge.edge_cost;
        
        // If this path is shorter, update the distance and save on the queue the destination node
        if (new_cost < distances[edge.dst_vertex_id]) {
          distances[edge.dst_vertex_id] = new_cost;
          predecessors[edge.dst_vertex_id] = current_node;
          edge_used[edge.dst_vertex_id] = edge.edge_id;
          pq.emplace(new_cost, edge.dst_vertex_id);
        }
      }
    }
    // Exception in case of not connected nodes
    
    if (distances[dest_node_id] == std::numeric_limits<cost_type>::max()) {
      throw std::runtime_error("No path found");
    }

    // Trace back from the destination node to the source node
    
    std::vector<id_type> nodes, edges;
    for (id_type current = dest_node_id; current != src_node_id; current = predecessors[current]) {
      nodes.push_back(current);
      edges.push_back(edge_used[current]);
    }
    nodes.push_back(src_node_id);

    // Reverse the order of nodes and edges to start from the source
    std::reverse(nodes.begin(), nodes.end());
    std::reverse(edges.begin(), edges.end());

    // return the nodes (ids) and weights
    return std::make_tuple(nodes, edges);
  }

private:
  struct ConnectionListItem {
    id_type dst_vertex_id; // Destination vertex
    id_type edge_id;       // Edge ID
    cost_type edge_cost;   // Edge cost
  };

  std::size_t id_{1}; // ID generator
  std::unordered_map<id_type, std::vector<ConnectionListItem>> graph_; // Graph representation

  // Generates unique IDs.
  std::size_t make_id() { return id_++; }
};

using IdVector = std::vector<ShortestPathCalculator::id_type>;

TEST(LifeFindsAWay, ConstructOk) {
  [[maybe_unused]] ShortestPathCalculator uut;
}

TEST(LifeFindsAWay, AddUniqueVertexIds) {
  // add two vertices and make sure they have different ids
  ShortestPathCalculator uut;
  auto id1 = uut.add_vertex();
  auto id2 = uut.add_vertex();
  ASSERT_NE(id1, id2);
}

TEST(LifeFindsAWay, AddUniqueEdgeIds) {
  // add three edges and make sure they have different ids
  ShortestPathCalculator uut;
  auto id1 = uut.add_vertex();
  auto id2 = uut.add_vertex();
  auto id3 = uut.add_vertex();
  auto id4 = uut.add_vertex();

  auto edge1 = uut.add_edge(id1, id2, 1);
  auto edge2 = uut.add_edge(id2, id3, 1);
  auto edge3 = uut.add_edge(id3, id4, 1);

  ASSERT_NE(edge1, edge2);
  ASSERT_NE(edge2, edge3);
}

TEST(LifeFindsAWay, AddEdgeInvalidSourceVertexThrows) {
  // invalid vertex throws
  ShortestPathCalculator uut;
  auto id1 = uut.add_vertex();
  auto id2 = uut.add_vertex();
  ASSERT_THROW(uut.add_edge(id1 + 10, id2, 1), std::runtime_error);
  ASSERT_THROW(uut.add_edge(id1, id2 + 10, 1), std::runtime_error);
  ASSERT_THROW(uut.add_edge(id1 + 10, id2 + 10, 1), std::runtime_error);
}

TEST(LifeFindsAWay, ShortestPathWithOneself) {
  // create a graph with a single vertex and test that the shortest path is
  // correct
  ShortestPathCalculator uut;
  auto id1 = uut.add_vertex();
  auto [nodes, edges] = uut.shortest_path(id1, id1);

  const auto expected_nodes = IdVector{id1};
  const auto expected_edges = IdVector{};

  ASSERT_EQ(nodes, expected_nodes);
  ASSERT_EQ(edges, expected_edges);
}

TEST(LifeFindsAWay, ShortestPathMethodIsConst) {
  // test that the shortest_path method is const
  ShortestPathCalculator uut;
  auto id1 = uut.add_vertex();
  const auto &const_uut = uut;
  auto [nodes, edges] = const_uut.shortest_path(id1, id1);
  ASSERT_EQ(nodes.size(), 1);
  ASSERT_EQ(edges.size(), 0);
}

TEST(LifeFindsAWay, ShortestPathTwoVerticesOneEdge) {
  // create a graph with two vertices and test that the shortest path is correct
  ShortestPathCalculator uut;
  auto id1 = uut.add_vertex();
  auto id2 = uut.add_vertex();
  auto edge1 = uut.add_edge(id1, id2, 1);
  auto [nodes, edges] = uut.shortest_path(id1, id2);

  const auto expected_nodes = IdVector{id1, id2};
  const auto expected_edges = IdVector{edge1};

  ASSERT_EQ(nodes, expected_nodes);
  ASSERT_EQ(edges, expected_edges);
}

TEST(LifeFindsAWay, ShortestPathTwoVerticesMultipleEdges) {
  // create a graph with two vertices and multiple edges with different costs
  // and test that the shortest path is correct
  ShortestPathCalculator uut;
  auto id1 = uut.add_vertex();
  auto id2 = uut.add_vertex();
  auto edge1 = uut.add_edge(id1, id2, 3);
  auto edge2 = uut.add_edge(id1, id2, 1);
  auto edge3 = uut.add_edge(id1, id2, 2);

  auto [nodes, edges] = uut.shortest_path(id1, id2);

  const auto expected_nodes = IdVector{id1, id2};
  const auto expected_edges = IdVector{edge2};

  ASSERT_EQ(nodes, expected_nodes);
  ASSERT_EQ(edges, expected_edges);
}

class ComplexGraph : public ::testing::Test {
protected:
  void SetUp() override {
    // create six connected vertices and one pair disconnected from the rest
    v1 = uut.add_vertex();
    v2 = uut.add_vertex();
    v3 = uut.add_vertex();
    v4 = uut.add_vertex();
    v5 = uut.add_vertex();
    v6 = uut.add_vertex();
    v7 = uut.add_vertex();
    v8 = uut.add_vertex();

    // create edges between them
    e12 = uut.add_edge(v1, v2, 1);
    e13 = uut.add_edge(v1, v3, 1);
    e23 = uut.add_edge(v2, v3, 1);
    e24 = uut.add_edge(v2, v4, 10);
    e34 = uut.add_edge(v3, v4, 5);
    e36 = uut.add_edge(v3, v6, 3);
    e45 = uut.add_edge(v4, v5, 2);
    e56 = uut.add_edge(v5, v6, 11);
    e64 = uut.add_edge(v6, v4, 1);
    e78 = uut.add_edge(v7, v8, 1);
    e87 = uut.add_edge(v8, v7, 1);
  }

  ShortestPathCalculator uut;
  std::size_t v1, v2, v3, v4, v5, v6, v7, v8;
  std::size_t e12, e13, e23, e24, e34, e36, e45, e56, e64, e78, e87;
};

TEST_F(ComplexGraph, TestAdjacency) {
  // test the connection between adjacent nodes
  auto test_adjacent_pair = [this](const auto n1, const auto n2,
                                   const auto expected_nodes,
                                   const auto expected_edges) {
    auto [nodes, edges] = uut.shortest_path(n1, n2);
    EXPECT_EQ(nodes, expected_nodes);
    EXPECT_EQ(edges, expected_edges);
  };

  test_adjacent_pair(v1, v2, IdVector{v1, v2}, IdVector{e12});
  test_adjacent_pair(v1, v3, IdVector{v1, v3}, IdVector{e13});
  test_adjacent_pair(v2, v3, IdVector{v2, v3}, IdVector{e23});
  test_adjacent_pair(v2, v4, IdVector{v2, v3, v6, v4}, IdVector{e23, e36, e64});
  test_adjacent_pair(v3, v4, IdVector{v3, v6, v4}, IdVector{e36, e64});
  test_adjacent_pair(v3, v6, IdVector{v3, v6}, IdVector{e36});
  test_adjacent_pair(v4, v5, IdVector{v4, v5}, IdVector{e45});
  test_adjacent_pair(v5, v6, IdVector{v5, v6}, IdVector{e56});
  test_adjacent_pair(v6, v4, IdVector{v6, v4}, IdVector{e64});
  test_adjacent_pair(v7, v8, IdVector{v7, v8}, IdVector{e78});
}

TEST_F(ComplexGraph, TestPathWithMultipleRewrites) {
  // test a path through to the graph that would require multiple rewrites of
  // the total cost of a given dst_node_id
  auto [nodes, edges] = uut.shortest_path(v1, v5);
  const auto expected_nodes = IdVector{v1, v3, v6, v4, v5};
  const auto expected_edges = IdVector{e13, e36, e64, e45};
  EXPECT_EQ(nodes, expected_nodes);
  EXPECT_EQ(edges, expected_edges);
}

TEST_F(ComplexGraph, TestPathBetweenNodesInALoop) {
  // test path between nodes involved in loop
  auto [nodes, edges] = uut.shortest_path(v4, v6);
  const auto expected_nodes = IdVector{v4, v5, v6};
  const auto expected_edges = IdVector{e45, e56};
  EXPECT_EQ(nodes, expected_nodes);
  EXPECT_EQ(edges, expected_edges);
}

TEST_F(ComplexGraph, TestPathBetweenNodesInDisconnectedSegments) {
  // test path between nodes in disconnected segments
  ASSERT_THROW(uut.shortest_path(v1, v8), std::runtime_error);
}

TEST_F(ComplexGraph, TestPathBetweenNodesWithNoPathBetweenThem) {
  // test path between nodes in disconnected segments
  ASSERT_THROW(uut.shortest_path(v5, v1), std::runtime_error);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
