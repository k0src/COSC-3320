import { PulseLoader } from "react-spinners";
import { FaTriangleExclamation } from "react-icons/fa6";
import styles from "./Controls.module.css";
import classNames from "classnames";

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
  const matrixSizes = [
    { value: 128, className: styles.btnSafe },
    { value: 256, className: styles.btnSafe },
    { value: 512, className: styles.btnSafe },
    { value: 1024, className: styles.btnSafe },
    { value: 2048, className: styles.btnSafe },
    { value: 4096, className: styles.btnWarning },
    { value: 8192, className: styles.btnWarning },
    { value: 16384, className: styles.btnWarningStrong },
    { value: 32768, className: styles.btnDanger },
  ];

  return (
    <>
      <div className={styles.controls}>
        <div className={styles.matrixButtons}>
          <h3 className={styles.controlsTitle}>Matrix Size</h3>
          <div className={styles.buttonContainer}>
            {matrixSizes.map((size) => (
              <button
                key={size.value}
                onClick={() => onRunTest(size.value)}
                disabled={running}
                className={classNames(styles.button, size.className)}
              >
                {size.value}
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
