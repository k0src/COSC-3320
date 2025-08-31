#include <iostream>
#include <functional>
#include <chrono>
#include <vector>
#include <iomanip>
#include <random>
#include <sstream>

// n x n matrix represented as a contiguous block
struct Matrix {
  int n;
  int* data;

  Matrix(int n) : n(n) {
    data = new int[n * n]();
  }

  ~Matrix() {
    delete[] data;
  }

  Matrix(const Matrix& other) : n(other.n) {
    data = new int[n * n];
    std::copy(other.data, other.data + n * n, data);
  }

  Matrix& operator=(const Matrix& other) {
    if (this != &other) {
      delete[] data;
      n = other.n;
      data = new int[n * n];
      std::copy(other.data, other.data + n * n, data);
    }
    return *this;
  }
};

void fillWithRandomValues(Matrix& A) {
  int n = A.n;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, 100);
  
  for (int i = 0; i < n * n; i++) {
    A.data[i] = dis(gen);
  }
}

// Row-major matrix addition
Matrix addRowMajor(const Matrix& A, const Matrix& B) {
  int n = A.n;
  Matrix C(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      C.data[i * n + j] = A.data[i * n + j] + B.data[i * n + j];
    }
  }
  return C;
}

// Column-major matrix addition
Matrix addColumnMajor(const Matrix& A, const Matrix& B) {
  int n = A.n;
  Matrix C(n);
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i++) {
      C.data[i * n + j] = A.data[i * n + j] + B.data[i * n + j];
    }
  }
  return C;
}

using addFunc = std::function<Matrix(const Matrix&, const Matrix&)>;
using timePoint = std::chrono::high_resolution_clock::time_point;

double getAverageFunctionExecutionTime(addFunc f, int numberOfExecutions, const Matrix& A, const Matrix& B) {
  double totalTime = 0.0;

  for (int i = 0; i < 100; i++) {
    Matrix warmup = f(A, B);
  }

  for (int i = 0; i < numberOfExecutions; i++) {
    timePoint start = std::chrono::high_resolution_clock::now();
    Matrix result = f(A, B);  

    timePoint end = std::chrono::high_resolution_clock::now();
    totalTime += std::chrono::duration<double, std::milli>(end - start).count();
  }

  return totalTime / numberOfExecutions;
}

struct ScaledTime {
  double value;
  std::string unit;
};

ScaledTime scaleTime(double ms) {
  ScaledTime t;
  if (ms < 1000.0) {
    t.value = ms;
    t.unit = "ms";
  } else if (ms < 60000.0) {
    t.value = ms / 1000.0;
    t.unit = "s";
  } else if (ms < 3600000.0) {
    t.value = ms / 60000.0;
    t.unit = "min";
  } else {
    t.value = ms / 3600000.0;
    t.unit = "h";
  }
  return t;
}


void printMatrixAdditionTimings(std::vector<int> nValues, int numberOfExecutions) {
  std::cout << "Matrix Addition Timings\n";
  std::cout << "--------------------------------------------------------\n";
  std::cout << std::setw(10) << std::right << "n" << " | "
            << std::setw(20) << std::right << "Row-major" << " | "
            << std::setw(20) << std::right << "Column-major" << std::endl;
  std::cout << "--------------------------------------------------------\n";

  for (int i = 0; i < nValues.size(); i++) {
    int n = nValues[i];
    Matrix A(n), B(n);
    fillWithRandomValues(A);
    fillWithRandomValues(B);
    
    double rowTime = 
      getAverageFunctionExecutionTime(addRowMajor, numberOfExecutions, A, B);
    double colTime = 
      getAverageFunctionExecutionTime(addColumnMajor, numberOfExecutions, A, B);
    
    ScaledTime scaledRowTime = scaleTime(rowTime);
    ScaledTime scaledColTime = scaleTime(colTime);
    
    std::ostringstream rowStr, colStr;
    rowStr << std::fixed << std::setprecision(6) 
           << scaledRowTime.value << " " << scaledRowTime.unit;
    colStr << std::fixed << std::setprecision(6) 
           << scaledColTime.value << " " << scaledColTime.unit;
    
    std::cout << std::setw(10) << std::right << n << " | "
              << std::setw(20) << std::right << rowStr.str() << " | "
              << std::setw(20) << std::right << colStr.str() << std::endl;
  }

  std::cout << "--------------------------------------------------------\n\n";
}

int main() {
    std::vector<int> nValues = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    int numberOfExecutions = 10; 
    
    std::cout << "\nTesting n values: ";
    for (size_t i = 0; i < nValues.size(); i++) {
        std::cout << nValues[i];
        if (i < nValues.size() - 1) std::cout << ", ";
    }
    std::cout << "\nExecutions per test: " << numberOfExecutions << "\n\n";
    
    printMatrixAdditionTimings(nValues, numberOfExecutions);
    return 0;
}
