#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <random>
#include <cstdint>

using data_item = std::tuple<int, int, int>;
using matrix = std::vector<std::vector<int>>;

// Random number generator
int gen_random_number(int max) {
  static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(0, max);
  return dist(rng);
}

// Creates an n x n matrix initialized with zeros
matrix create_matrix(int n) {
  return matrix(n, std::vector<int>(n, 0));
}

// Apply a single update 
inline void apply_update(matrix& mat, int row, int col, int value) {
  mat[row][col] += value;
}

int main() 
{
  std::vector<int> n_values = {16, 64, 256, 1024, 4096, 16384};
  std::vector<long long> m_values = {1677721600LL, 13421772800LL};

  for (size_t i = 0; i < n_values.size(); ++i) {
    for (size_t j = 0; j < m_values.size(); ++j) {
      int n = n_values[i];
      long long m = m_values[j];

      matrix mat = create_matrix(n);

      std::cout << "Running test with n: " << n << ", and m: " << m << std::endl;

      auto start = std::chrono::high_resolution_clock::now();

      for (long long t = 0; t < m; ++t) {
        int row = gen_random_number(n - 1);
        int col = gen_random_number(n - 1);
        int value = gen_random_number(100); 
        apply_update(mat, row, col, value);
      }

      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = end - start;

      std::cout << "n: " << n 
                << ", m: " << m 
                << ", Time: " << elapsed.count() << " seconds" 
                << std::endl;
    }
  }

  return 0;
}
