#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

int main()
{
  vector<long long> n_values = {16, 64, 256, 1024, 4096, 16384};
  vector<long long> m_values = {1677721600LL, 13421772800LL};

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
      long long n_squared = n * n;

      double matrix_size_mb = (static_cast<double>(n) * n * 8.0) / (1024.0 * 1024.0);
      double m_over_n2 = static_cast<double>(m) / static_cast<double>(n_squared);

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

        double ops_per_sec = m / elapsed.count();
        double ops_per_sec_millions = ops_per_sec / 1000000.0;

        cout << setw(8) << n
             << setw(12) << n_squared
             << setw(15) << matrix_size_mb
             << setw(15) << elapsed.count()
             << setw(18) << ops_per_sec_millions
             << setw(12) << fixed << setprecision(1) << m_over_n2 << "\n";
      }
      catch (bad_alloc &e) {
        cout << setw(8) << n
             << setw(12) << n_squared
             << setw(15) << matrix_size_mb
             << " Could not allocate matrix (insufficient memory)\n";
      }
    }
  }
  return 0;
}