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

#include "gtest/gtest.h"

/// BEGIN EDIT ------------------------------------------------------

//
// This block may or may not be necessary depending on your implementation.
// Feel free to use it if you need it, or delete it if you don't.
//

/// END EDIT --------------------------------------------------------

class ShortestPathCalculator {
public:
  using id_type = std::size_t;
  using cost_type = std::size_t;

  // Add a vertex to the graph. Return a unique id for the vertex.
  id_type add_vertex() {
    auto id = make_id();
    graph_.emplace(id, std::vector<ConnectionListItem>{});
    return id;
  }

  // Add an edge_id between a pair of vertices with a edge_cost to go
  // srd_node_id the first to the second. Return a unique id for the edge_id.
  id_type add_edge(id_type from, id_type to, cost_type edge_cost) {
    if (graph_.find(from) == graph_.end()) {
      throw std::runtime_error("invalid source vertex");
    }
    if (graph_.find(to) == graph_.end()) {
      throw std::runtime_error("invalid destination vertex");
    }
    auto edge_id = make_id();
    graph_[from].emplace_back(ConnectionListItem{to, edge_id, edge_cost});
    return edge_id;
  }

  // Calculate the shortest path between two vertices whose ids are given.
  // Returns a tuple of two vectors, the first containing the sequence of nodes
  // and the second containing the sequence of edges that make up the path.
  // If no path is found between the two vertices, throw a std::runtime_error.
  auto shortest_path(id_type src_node_id, id_type dest_node_id) const {
    std::vector<id_type> nodes;
    std::vector<id_type> edges;

    /// BEGIN EDIT ------------------------------------------------------

    // Implement Dijkstra's algorithm here. You can use std::priority_queue.

    /// END EDIT --------------------------------------------------------

    return std::make_tuple(nodes, edges);
  }

private:
  struct ConnectionListItem {
    id_type dst_vertex_id;
    id_type edge_id;
    cost_type edge_cost;
  };

  std::size_t id_{1}; // 0 can be used for "invalid id"

  std::unordered_map<id_type, std::vector<ConnectionListItem>> graph_;

  // determine a unique id for a vertex
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
