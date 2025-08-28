import { useState } from "react";
import styles from "./App.module.css";
import Algorithms from "./components/Algorithms/Algorithms";
import { Controls } from "./components/Controls/Controls";
import { Results } from "./components/Results/Results";

interface TimingResult {
  rowMajor: number;
  colMajor: number;
  matrixSize: number;
  executions: number;
}

function App() {
  const [running, setRunning] = useState(false);
  const [executions, setExecutions] = useState(3);
  const [results, setResults] = useState<TimingResult | null>(null);
  const [showWarning, setShowWarning] = useState(false);

  const checkWarning = (size: number, execs: number) => {
    setShowWarning(size >= 8192 || execs > 10);
  };

  const handleExecutionsChange = (value: number) => {
    setExecutions(value);
    if (results) {
      checkWarning(results.matrixSize, value);
    }
  };

  // Row-major matrix addition
  const rowMajorAdd = (
    a: Float32Array,
    b: Float32Array,
    c: Float32Array,
    n: number
  ) => {
    for (let i = 0; i < n; i++) {
      for (let j = 0; j < n; j++) {
        const idx = i * n + j;
        c[idx] = a[idx] + b[idx];
      }
    }
  };

  // Column-major matrix addition
  const colMajorAdd = (
    a: Float32Array,
    b: Float32Array,
    c: Float32Array,
    n: number
  ) => {
    for (let j = 0; j < n; j++) {
      for (let i = 0; i < n; i++) {
        const idx = i * n + j;
        c[idx] = a[idx] + b[idx];
      }
    }
  };

  const runTest = async (n: number) => {
    setRunning(true);
    setResults(null);
    checkWarning(n, executions);

    await new Promise((resolve) => setTimeout(resolve, 100));

    const size = n * n;

    // Create matrices
    const a = new Float32Array(size);
    const b = new Float32Array(size);
    const c = new Float32Array(size);

    // Fill with random numbers
    for (let i = 0; i < size; i++) {
      a[i] = Math.random();
      b[i] = Math.random();
    }

    // Test row-major
    const rowStart = performance.now();
    for (let i = 0; i < executions; i++) {
      rowMajorAdd(a, b, c, n);
    }
    const rowEnd = performance.now();
    const rowMajorTime = rowEnd - rowStart;

    c.fill(0);

    // Test column-major
    const colStart = performance.now();
    for (let i = 0; i < executions; i++) {
      colMajorAdd(a, b, c, n);
    }
    const colEnd = performance.now();
    const colMajorTime = colEnd - colStart;

    setResults({
      rowMajor: rowMajorTime,
      colMajor: colMajorTime,
      matrixSize: n,
      executions,
    });
    setRunning(false);
  };

  return (
    <div className={styles.container}>
      <h1 className={styles.headingPrimary}>Matrix Addition Algorithms Demo</h1>

      <div className={styles.content}>
        <Algorithms />
        <div className={styles.contentBottom}>
          <Controls
            running={running}
            executions={executions}
            showWarning={showWarning}
            onExecutionsChange={handleExecutionsChange}
            onRunTest={runTest}
          />

          <Results results={results} running={running} />
        </div>
      </div>
    </div>
  );
}

export default App;
