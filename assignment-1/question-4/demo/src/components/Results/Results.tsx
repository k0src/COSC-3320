import styles from "./Results.module.css";

interface TimingResult {
  rowMajor: number;
  colMajor: number;
  matrixSize: number;
  executions: number;
}

interface ResultsProps {
  results: TimingResult | null;
  running: boolean;
}

export function Results({ results, running }: ResultsProps) {
  if (!results || running) return null;

  return (
    <div className={styles.resultsContainer}>
      <h3 className={styles.resultsTitle}>
        Results for matrix of size{" "}
        <span className={styles.resultsMono}>{results.matrixSize}</span> with{" "}
        <span className={styles.resultsMono}>{results.executions}</span>{" "}
        executions
      </h3>

      <div className={styles.timings}>
        <div className={styles.timing}>
          Row-major:{" "}
          <span className={styles.resultsMono}>
            {results.rowMajor.toFixed(2)}ms
          </span>
        </div>
        <div className={styles.timing}>
          Column-major:{" "}
          <span className={styles.resultsMono}>
            {results.colMajor.toFixed(2)}ms
          </span>
        </div>
      </div>
    </div>
  );
}
