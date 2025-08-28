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

using timingResult = std::pair<int, double>;
using searchFunction = std::function<int(std::vector<int>&, int)>;
using timePoint = std::chrono::high_resolution_clock::time_point;

timingResult getAverageExecutionTime(searchFunction func, std::vector<int>& array, int target, int executions) {
  int index = -1;

  // Warmup
  for (int i = 0; i < std::max(10000, executions); i++) {
    index = func(array, target);
  }

  timePoint start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < executions; i++) {
    index = func(array, target);
  }
  timePoint end = std::chrono::high_resolution_clock::now();

  double averageExecutionTime = std::chrono::duration<double, std::nano>(end - start).count() / executions;
  return {index, averageExecutionTime};
}

void runTest(int executions) {
  std::vector<int> arraySizes = {100, 400, 1600, 6400, 25600, 102400, 409600, 1638400, 6553600};

  for (int i = 0; i < arraySizes.size(); i++) {
    int size = arraySizes[i];
    std::vector<int> array = createBinarySearchableArray(size);
    int target = size + 1; // Element not in array

    auto [index, averageExecutionTime] = getAverageExecutionTime(
      binarySearch,
      array,
      target,
      executions
    );

    std::cout << "Array Size: " << size << ", Binary Search " << (index == -1 ? "Unsuccessful" : "Successful")
              << ", Average Execution Time: " << std::fixed << std::setprecision(3) 
              << averageExecutionTime << " (ns)\n";
  }
}


int main(int argc, char* argv[]) 
{
  int executions = std::stoi(argv[1]);
  runTest(executions);
  return 0;
}