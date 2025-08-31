#include <iostream>
#include <functional>
#include <chrono>
#include <vector>
#include <iomanip>

int binarySearch(const std::vector<int>& array, int target) {
  int left = 0;
  int right = array.size() - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;

    if (array[mid] == target) {
      return mid;
    } else if (array[mid] < target) {
      left = mid + 1; 
    } else {
      right = mid - 1; 
    }
  }

  return -1;
}

std::vector<int> createBinarySearchableArray(int size) {
  std::vector<int> array = std::vector<int>(size);
  for (int i = 0; i < size; i++) {
    array[i] = i;
  }
  return array;
}

using searchFunction = std::function<int(std::vector<int>&, int)>;
using timePoint = std::chrono::high_resolution_clock::time_point;

double getAverageExecutionTime(searchFunction func, std::vector<int>& array, int target, int executions) {
  int index = -1;
  double totalTime = 0.0;

  // Warmup
  for (int i = 0; i < std::max(10000, executions); i++) {
    index = func(array, target);
  }
  
  for (int i = 0; i < executions; i++) {
    timePoint start = std::chrono::high_resolution_clock::now();

    for (int j = 0; j < 30000000; j++) {
      index = func(array, target);
    }

    timePoint end = std::chrono::high_resolution_clock::now();
    totalTime += std::chrono::duration<double, std::nano>(end - start).count();
  }

  return totalTime / executions / 1000000000; // Convert to seconds
}

void runTests(int executions) {
  std::vector<int> arraySizes = {100, 400, 1600, 6400, 25600, 102400, 409600, 1638400};

  for (int i = 0; i < arraySizes.size(); i++) {
    int size = arraySizes[i];
    std::vector<int> array = createBinarySearchableArray(size);
    int target = size + 1; // Element not in array

    double averageExecutionTime = getAverageExecutionTime(
      binarySearch,
      array,
      target,
      executions
    );

    std::cout << "Array Size: " << size 
              << ", Average Execution Time for 30,000,000 Unsuccessful Searches: " 
              << std::fixed << std::setprecision(3) << averageExecutionTime << " (s)" 
              << std::endl;
  }
}

int main(int argc, char* argv[]) 
{
  runTests(std::stoi(argv[1]));
  return 0;
} 
