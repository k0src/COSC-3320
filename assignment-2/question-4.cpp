#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

struct TestResult {
  long long n;
  long long n_squared;
  double matrix_size_mb;
  double elapsed_time;
  double ops_per_sec_millions;
  double m_over_n2;
  bool success;
};

TestResult run_test(long long n, long long m) {
  TestResult result;
  result.n = n;
  result.n_squared = n * n;
  result.matrix_size_mb = (static_cast<double>(n) * n * 8.0) / (1024.0 * 1024.0);
  result.m_over_n2 = static_cast<double>(m) / static_cast<double>(result.n_squared);
  result.success = false;

  try {
    vector<vector<double>> M(n, vector<double>(n, 0.0));

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist_index(1, n);
    uniform_real_distribution<double> dist_value(-1.0, 1.0);

    auto start = high_resolution_clock::now();

    for (long long k = 0; k < m; k++) {
      long long i = dist_index(gen);
      long long j = dist_index(gen);
      double x = dist_value(gen);
      M[i - 1][j - 1] += x;
    }

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    result.elapsed_time = elapsed.count();
    double ops_per_sec = m / elapsed.count();
    result.ops_per_sec_millions = ops_per_sec / 1000000.0;
    result.success = true;
  }
  catch (bad_alloc &e) {
    result.success = false;
  }

  return result;
}

void print_results(const vector<long long> &n_values, const vector<long long> &m_values) {
  cout << fixed << setprecision(3);

  for (long long m : m_values) {
    cout << "\nResults for m = " << m << " update operations\n";
    cout << string(90, '=') << "\n";
    cout << setw(8) << "n"
         << setw(12) << "n^2"
         << setw(15) << "Matrix (MB)"
         << setw(15) << "Time (sec)"
         << setw(18) << "Ops/sec (M)"
         << setw(12) << "m/n^2\n";
    cout << string(90, '-') << "\n";

    for (long long n : n_values) {
      TestResult result = run_test(n, m);

      if (result.success) {
        cout << setw(8) << result.n
             << setw(12) << result.n_squared
             << setw(15) << result.matrix_size_mb
             << setw(15) << result.elapsed_time
             << setw(18) << result.ops_per_sec_millions
             << setw(12) << fixed << setprecision(1) << result.m_over_n2 << "\n";
      }
      else {
        cout << setw(8) << result.n
             << setw(12) << result.n_squared
             << setw(15) << result.matrix_size_mb
             << " Could not allocate matrix (insufficient memory)\n";
      }
    }
  }
}

int main() {
  vector<long long> n_values = {16, 64, 256, 1024, 4096, 16384};
  vector<long long> m_values = {1677721600LL, 13421772800LL};

  print_results(n_values, m_values);

  return 0;
}