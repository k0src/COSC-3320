import time
import sys

def binarySearch(array, target):
  left = 0
  right = len(array) - 1

  while left <= right:
    mid = left + (right - left) // 2

    if array[mid] == target:
      return mid
    elif array[mid] < target:
      left = mid + 1
    else:
      right = mid - 1

  return -1

def createBinarySearchableArray(size):
  array = list(range(size))
  return array

def getAverageExecutionTime(func, array, target, executions):
  index = -1
  totalTime = 0.0

  # Warmup
  for _ in range(max(10000, executions)):
    index = func(array, target)

  for _ in range(executions):
    start = time.perf_counter_ns()

    for _ in range(30_000_000):
      index = func(array, target)
    
    end = time.perf_counter_ns()
    totalTime += (end - start)

  return totalTime / executions / 1_000_000_000  # Convert to seconds


def runTests(executions):
  arraySizes = [100, 400, 1600, 6400, 25600, 102400, 409600, 1638400]

  for size in arraySizes:
    array = createBinarySearchableArray(size)
    target = size + 1 # Element not in array

    averageExecutionTime = getAverageExecutionTime(
      binarySearch,
      array,
      target,
      executions
    )

    print(f"Array size: {size}, Average Execution Time for 30,000,000 Unsuccessful Searches: {averageExecutionTime:.3f} (s)")

if __name__ == "__main__":
  executions = int(sys.argv[1])
  runTests(executions)