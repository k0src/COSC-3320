#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <queue>


using Vertex = char;
using Edge = std::pair<Vertex, int>; // (neighbor, weight)
using Graph = std::unordered_map<Vertex, std::vector<Edge>>; 
using DVPair = std::pair<int, Vertex>; // (distance, vertex)

// For result printing
struct DijkstraGraph {
  DijkstraGraph(const Graph& g, const Vertex& start, const std::string& description)
    : g(g), start(start), description(description) {}
  Graph g;
  Vertex start;
  std::string description;
};

std::unordered_map<Vertex, int> dijkstra(const Graph& g, const Vertex start) {
  // Map containing each node and its distance from the start node
  std::unordered_map<Vertex, int> distances;
  std::unordered_set<Vertex> visited;

  // Set all distances to infinity
  for (const auto& [v, _] : g) {
    distances[v] = std::numeric_limits<int>::max();
  }

  // Create a min-heap to store (distance, vertex) pairs  
  std::priority_queue<DVPair, std::vector<DVPair>, std::greater<>> pq;

  // Initialize start node
  distances[start] = 0;
  pq.push({0, start});

  while (!pq.empty()) {
    int dist = pq.top().first; // current shortest distance
    Vertex u = pq.top().second; // current vertex
    pq.pop();

    // If we have already visited this node, skip it
    if (visited.contains(u)) {
      continue;
    }

    visited.insert(u);

    // Compute the distance to each neighbor 
    // and update if a shorter path is found
    for (const auto& [v, weight] : g.at(u)) {
      if (!visited.contains(v) && dist + weight < distances[v]) {
        distances[v] = dist + weight;
        pq.push({distances[v], v}); 
      }
    }
  }

  return distances;
}

int main()
{
  std::vector<DijkstraGraph> test_graphs = {
    {
      {
        {'A', { {'B', 1}, {'C', 4} }},
        {'B', { {'C', 2}, {'D', 5} }},
        {'C', { {'D', 1} }},
        {'D', {} }
      },
      'A',
      "Basic acyclic graph"
    },
    {
      {
        {'A', { {'B', 5} }},
        {'B', { {'C', 7} }},
        {'C', {} },
        {'D', { {'E', 1} }},
        {'E', {} }
      },
      'A',
      "Graph with disconnected components"
    },
    {
      {
        {'A', {} }
      },
      'A',
      "Single-vertex graph"
    },
    {
      {
        {'A', { {'B', 2}, {'C', 2} }},
        {'B', { {'D', 2} }},
        {'C', { {'D', 2} }},
        {'D', {} }
      },
      'A',
      "Graph with equal-weight multiple paths"
    },
    {
      {
        {'A', { {'B', 0}, {'C', 5} }},
        {'B', { {'C', 2} }},
        {'C', { {'D', 3} }},
        {'D', {} }
      },
      'A',
      "Graph containing a zero-weight edge"
    },
    {
      {
        {'A', { {'B', 2}, {'C', 5}, {'D', 1} }},
        {'B', { {'C', 2}, {'E', 4} }},
        {'C', { {'E', 1}, {'F', 7} }},
        {'D', { {'C', 1}, {'F', 4} }},
        {'E', { {'F', 1} }},
        {'F', {} }
      },
      'A',
      "Dense graph with multiple competing paths"
    },
    {
      {
        {'A', { {'B', 4}, {'C', 2} }},
        {'B', { {'D', 5} }},
        {'C', { {'E', 10} }},
        {'D', {} },
        {'E', {} },
        {'F', {} }
      },
      'A',
      "Graph containing dead-end vertices"
    },
    {
      {
        {'A', { {'B', 1}, {'C', 4} }},
        {'B', { {'C', 2}, {'A', 1} }},
        {'C', { {'D', 1}, {'B', 2} }},
        {'D', { {'E', 3} }},
        {'E', { {'B', 5} }}
      },
      'A',
      "Graph containing multiple cycles"
    },
    {
      {
        {'A', { {'B', 1} }},
        {'B', { {'C', 1} }},
        {'C', { {'D', 1} }},
        {'D', { {'E', 1} }},
        {'E', { {'F', 1} }},
        {'F', {} }
      },
      'A',
      "Long linear chain"
    },
    {
      {
        {'A', { {'B', 100000000}, {'C', 200000000} }},
        {'B', { {'D', 150000000} }},
        {'C', { {'D', 50000000} }},
        {'D', { {'E', 250000000} }},
        {'E', {} }
      },
      'A',
      "Graph with very large edge weights"
    }
  };

  for (const auto& test_graph : test_graphs) {
    std::cout << test_graph.description << std::endl;
    std::unordered_map<Vertex, int> distances = dijkstra(test_graph.g, test_graph.start);
    for (const auto& [v, d] : distances) {
      if (d == std::numeric_limits<int>::max()) {
        std::cout << "No path from " << test_graph.start << " to " << v << std::endl; 
      } else {
        std::cout << "Distance from " << test_graph.start << " to " << v << " = " << d << std::endl;
      }
    }
    std::cout << std::endl;
  }

  return 0;
}