// Binary search function
const binarySearch = (array: number[], target: number): number => {
  let left = 0;
  let right = array.length - 1;

  while (left <= right) {
    const mid = Math.floor((left + right) / 2);

    if (array[mid] === target) {
      return mid;
    } else if (array[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
};

const createBinarySearchableArray = (size: number): number[] => {
  const array = new Array(size);
  for (let i = 0; i < size; i++) {
    array[i] = i;
  }
  return array;
};

interface SearchFunction {
  (array: number[], target: number): number;
}

const getAverageExecutionTime = (
  func: SearchFunction,
  array: number[],
  target: number,
  executions: number
): number => {
  let index = -1;
  let totalTime = 0.0;

  // Warmup to trigger JIT optimization
  for (let i = 0; i < Math.max(10000, executions); i++) {
    index = func(array, target);
  }

  for (let i = 0; i < executions; i++) {
    const start = process.hrtime.bigint();

    for (let j = 0; j < 30_000_000; j++) {
      index = func(array, target);
    }

    const end = process.hrtime.bigint();
    totalTime += Number(end - start);
  }

  return totalTime / executions / 1_000_000_000; // Convert to seconds
};

const runTests = (executions: number) => {
  const arraySizes = [100, 400, 1600, 6400, 25600, 102400, 409600, 1638400];

  for (let i = 0; i < arraySizes.length; i++) {
    const size = arraySizes[i];
    const array = createBinarySearchableArray(size);
    const target = size + 1; // Element not in array

    const averageExecutionTime = getAverageExecutionTime(
      binarySearch,
      array,
      target,
      executions
    );

    console.log(
      `Array Size: ${size}, Average Execution Time for 30,000,000 Unsuccessful Searches: ${averageExecutionTime.toFixed(
        3
      )} (s)`
    );
  }
};

if (require.main === module) {
  const executions = Number(process.argv.slice(2)[0]);
  runTests(executions);
}
