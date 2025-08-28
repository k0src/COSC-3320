import styles from "./Algorithms.module.css";

export default function Algorithms() {
  return (
    <div className={styles.algorithms}>
      <div className={styles.algorithm}>
        <h3 className={styles.algorithmName}>Row-major</h3>
        <div className={styles.codeBlock}>
          <span>for i from 1 to n</span>
          <span className={styles.line2}>for j from 1 to n</span>
          <span className={styles.line3}>C[i, j] ← A[i, j] + B[i, j]</span>
        </div>
      </div>

      <div className={styles.algorithm}>
        <h3 className={styles.algorithmName}>Column-major</h3>
        <div className={styles.codeBlock}>
          <span>for j from 1 to n</span>
          <span className={styles.line2}>for i from 1 to n</span>
          <span className={styles.line3}>C[i, j] ← A[i, j] + B[i, j]</span>
        </div>
      </div>
    </div>
  );
}
