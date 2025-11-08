#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <cmath>
#include <windows.h>
#include <psapi.h>

struct Metrics {
  double elapsed_s;
  SIZE_T workingSet;
  DWORD pageFaults;
};

Metrics run_workload(size_t data_size_bytes, bool random_access, int iterations) {
  size_t n = data_size_bytes / sizeof(double);
  std::vector<double> data(n, 1.0);
  std::vector<size_t> indices(n);
  for (size_t i = 0; i < n; ++i) {
    indices[i] = i;
  }

  if (random_access) {
    std::mt19937 rng(42);
    std::shuffle(indices.begin(), indices.end(), rng);
  }

  volatile double sum = 0.0; 
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; i++) {
    for (size_t j = 0; j < n; j++) {
      data[indices[j]] += 1.0;
      sum += data[indices[j]];
    }
  }
  auto end = std::chrono::high_resolution_clock::now();

  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

  Metrics m;
  m.elapsed_s = std::chrono::duration<double>(end - start).count();
  m.workingSet = pmc.WorkingSetSize;
  m.pageFaults = pmc.PageFaultCount;
  return m;
}

void log_results(std::ofstream& out, double ratio, double size_gb, const Metrics& seq, const Metrics& rnd) {
  out << ratio << "," << size_gb << ","
      << seq.elapsed_s << "," << seq.workingSet << "," << seq.pageFaults << ","
      << rnd.elapsed_s << "," << rnd.workingSet << "," << rnd.pageFaults << "\n";
  out.flush();
}

int main() 
{
  const double M_bytes = 32.0 * 1024 * 1024 * 1024;
  std::vector<double> ratios = {0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 0.99, 1.0, 1.01, 1.1, 1.5, 2.0, 5.0, 10.0, 50.0};
  int iterations = 1;

  std::ofstream out("memory_scaling_results.csv");
  out << "C/M,Data_Size_GB,Seq_Time_s,Seq_WorkingSet_B,Seq_PageFaults,Rand_Time_s,Rand_WorkingSet_B,Rand_PageFaults\n";

  for (double ratio : ratios) {
    double data_size_bytes = M_bytes * ratio;
    double data_size_gb = data_size_bytes / (1024.0 * 1024.0 * 1024.0);
    std::cout << "C/M = " << ratio << " (" << data_size_gb << " GB)" << std::endl;

    try {
      Metrics seq = run_workload((size_t)data_size_bytes, false, iterations);
      Metrics rnd = run_workload((size_t)data_size_bytes, true, iterations);
      log_results(out, ratio, data_size_gb, seq, rnd);
    } catch (const std::bad_alloc&) {
      std::cerr << "Memory allocation failed at C/M = " << ratio << std::endl;
      Metrics empty = { -1, 0, 0 };
      log_results(out, ratio, data_size_gb, empty, empty);
    }
  }

  return 0;
}
