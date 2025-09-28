#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <fstream>

class HanoiGraphSolver {
public:
  HanoiGraphSolver(int n, const std::string& filename) : _n(n), moveCount(0) {
    // Open output file
    outputFile.open(filename);
    if (!outputFile.is_open()) {
      throw std::runtime_error("Failed to open output file");
    }

    // Define the graph using an adjacency list 
    graph["Start"] = {"A1"};
    graph["A1"] = {"Start", "Dest", "A3"};
    graph["A2"] = {"Dest", "A3"};
    graph["A3"] = {"A2", "A1"};
    graph["Dest"] = {"A1", "A2"};

    // Initialize towers
    for (const auto& pair : graph) {
      towers[pair.first] = {};
    }

    // Place disks on the Start tower
    for (int i = _n; i > 0; --i) {
      towers["Start"].push_back(i);
    }
  }

  ~HanoiGraphSolver() {
    if (outputFile.is_open()) {
      outputFile.close();
    }
  }

  void solve() {
    outputFile << "Solving Towers of Hanoi for " << _n << " disks" << std::endl;
    startToDest(_n);
    outputFile << "Total moves: " << moveCount << std::endl;
  }

private:
  void moveDisk(const std::string& src, const std::string& dst) {
    if (towers.at(src).empty()) {
      throw std::logic_error("Cannot move from empty tower");
    }
    
    int disk = towers.at(src).back();
    towers.at(src).pop_back();
    
    if (!towers.at(dst).empty() && towers.at(dst).back() < disk) {
      throw std::logic_error("Cannot place disk on smaller disk");
    }

    towers.at(dst).push_back(disk);
    moveCount++;
    outputFile << "Move " << moveCount 
              << ": Move disk " << disk << " from " << src 
              << " to " << dst << std::endl;
  }

  void startToDest(int n) {
    if (n == 0) return;
    startToDest(n - 1);
    moveNonAdjacent(n - 1, "Dest", "A3");
    moveDisk("Start", "A1");
    moveDisk("A1", "Dest");
    moveNonAdjacent(n - 1, "A3", "Dest");
  }

  void moveAdjacent(int n, const std::string& src, const std::string& dst) {
    if (n == 0) return;

    std::set<std::string> auxCandidates;
    std::set<std::string> srcSet = {src};
    std::set_difference(graph.at(dst).begin(), graph.at(dst).end(),
                        srcSet.begin(), srcSet.end(),
                        std::inserter(auxCandidates, auxCandidates.begin()));

    if (auxCandidates.empty()) {
      throw std::logic_error("No valid auxiliary peg found");
    }
    std::string aux = *auxCandidates.begin();

    moveNonAdjacent(n - 1, src, aux);
    moveDisk(src, dst);
    moveAdjacent(n - 1, aux, dst);
  }


  void moveNonAdjacent(int n, const std::string& src, const std::string& dst) {
    if (n == 0) return;
    
    std::set<std::string> neighbors;
    std::set_intersection(graph.at(src).begin(), graph.at(src).end(),
                          graph.at(dst).begin(), graph.at(dst).end(),
                          std::inserter(neighbors, neighbors.begin()));

    if (neighbors.empty()) {
      throw std::logic_error("No path of length 2 between " + src + " and " + dst);
    }
    std::string aux = *neighbors.begin();

    moveAdjacent(n, src, aux);
    moveAdjacent(n, aux, dst);
  }

  int _n;
  long long moveCount;
  std::map<std::string, std::set<std::string>> graph;
  std::map<std::string, std::vector<int>> towers;
  std::ofstream outputFile;
};

int main() {
  std::vector<int> nValues = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (int n : nValues) {
    HanoiGraphSolver solver(n, "hanoi_graph_solution_" 
                                + std::to_string(n) + 
                                "_disks.txt");
    solver.solve();
  }

  return 0;
}