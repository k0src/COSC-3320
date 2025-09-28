#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <iomanip>

class HanoiGraphSolver {
public:
  HanoiGraphSolver(int n) : _n(n), moveCount(0) {
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

  void solve() {
    std::cout << "Solving Towers of Hanoi for " << _n << " disks" << std::endl;
    startToDest(_n);
    std::cout << "Total moves: " << moveCount << std::endl;
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
    std::cout << "Move " << std::setw(5) << moveCount 
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
    std::string inter = *neighbors.begin();

    moveAdjacent(n, src, inter);
    moveAdjacent(n, inter, dst);
  }

  int _n;
  long long moveCount;
  std::map<std::string, std::set<std::string>> graph;
  std::map<std::string, std::vector<int>> towers;
};

int main() {
  int n = 5;

  HanoiGraphSolver solver(n);
  solver.solve();

  return 0;
}