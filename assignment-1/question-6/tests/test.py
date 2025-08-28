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

  # Warmup
  for _ in range(max(10000, executions)):
    index = func(array, target)

  start = time.perf_counter_ns()
  for _ in range(executions):
    index = func(array, target)
  end = time.perf_counter_ns()

  return {
    "index": index,
    "averageExecutionTime": (end - start) / executions,
  }

def runTest(executions):
  arraySizes = [100, 400, 1600, 6400, 25600, 102400, 409600, 1638400, 6553600]

  for size in arraySizes:
    array = createBinarySearchableArray(size)
    target = size + 1 # Element not in array

    index, averageExecutionTime = getAverageExecutionTime(
      binarySearch,
      array,
      target,
      executions
    ).values()

    print(f"Array size: {size}, Binary Search: {'Unsuccessful' if index == -1 else 'Successful'}, Average execution time: {averageExecutionTime:.3f} (ns)")

if __name__ == "__main__":
  executions = int(sys.argv[1])
  runTest(executions)