import { PulseLoader } from "react-spinners";
import { FaTriangleExclamation } from "react-icons/fa6";
import styles from "./Controls.module.css";

interface ControlsProps {
  running: boolean;
  executions: number;
  showWarning: boolean;
  onExecutionsChange: (value: number) => void;
  onRunTest: (size: number) => void;
}

export function Controls({
  running,
  executions,
  showWarning,
  onExecutionsChange,
  onRunTest,
}: ControlsProps) {
  const matrixSizes = [128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768];

  return (
    <>
      <div className={styles.controls}>
        <div className={styles.matrixButtons}>
          <h3 className={styles.controlsTitle}>Matrix Size</h3>
          <div className={styles.buttonContainer}>
            {matrixSizes.map((size) => (
              <button
                key={size}
                onClick={() => onRunTest(size)}
                disabled={running}
                className={styles.button}
              >
                {size}
              </button>
            ))}
          </div>
        </div>

        <div className={styles.inputContainer}>
          <span className={styles.label}>Number of executions:</span>
          <input
            id="executions"
            type="number"
            min="1"
            max="100"
            value={executions}
            onChange={(e) => onExecutionsChange(parseInt(e.target.value))}
            disabled={running}
            className={styles.input}
          />
        </div>
      </div>

      {showWarning && (
        <div className={styles.warning}>
          <FaTriangleExclamation className={styles.warningIcon} />
          Warning: Large matrix sizes or many executions may take a long time or
          crash the browser.
        </div>
      )}

      {running && (
        <div className={styles.loaderContainer}>
          <PulseLoader color="#e3e5e4" size="16px" />
        </div>
      )}
    </>
  );
}
