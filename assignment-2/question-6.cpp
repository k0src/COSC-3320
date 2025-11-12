#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <sys/resource.h>
#include <unistd.h>

struct Metrics {
  double elapsed_s;
  size_t rss_kb;
  long page_faults;
};

Metrics get_process_metrics(double elapseds_s) {
  struct rusage usage{};
  getrusage(RUSAGE_SELF, &usage);

  std::ifstream statm("/proc/self/statm");
  size_t size, resident;
  statm >> size >> resident;

  long page_size = sysconf(_SC_PAGESIZE);
  Metrics m{};
  m.elapsed_s = elapsed_s;
  m.rss_kb = resident * page_size / 1024;
  m.page_faults = usage.ru_majflt + usage.ru_minflt;
  return m;
}

Metrics run_workload(size_t data_size_bytes, bool random_access, int iterations) {
  long page_size = sysconf(_SC_PAGESIZE);
  size_t num_pages = data_size_bytes / page_size;

  std::vector<char> data(data_size_bytes, 1);
  std::vector<size_t> page_indices(num_pages);
  for (size_t i = 0; i < num_pages; ++i) page_indices[i] = i;

  if (random_access) {
    std::mt19937 rng(42);
    std::shuffle(page_indices.begin(), page_indices.end(), rng);
  }

  const int touches_per_page = 500;

  volatile unsigned long long sink = 0;
  auto start = std::chrono::high_resolution_clock::now();

  for (int it = 0; it < iterations; ++it) {
    for (int t = 0; t < touches_per_page; ++t) {
      for (size_t p = 0; p < num_pages; ++p) {
        size_t offset = page_indices[p] * page_size;
        data[offset] += 1;
        sink += data[offset];
      }
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  double elapsed = std::chrono::duration<double>(end - start).count();
  return get_process_metrics(elapsed);
}

void log_results(std::ofstream& out, double ratio, double size_gb,
                 const Metrics& seq, const Metrics& rnd) {
  out << ratio << "," << size_gb << ","
      << seq.elapsed_s << "," << seq.rss_kb << "," << seq.page_faults << ","
      << rnd.elapsed_s << "," << rnd.rss_kb << "," << rnd.page_faults << "\n";
  out.flush();
}

int main() 
{
  const double M_bytes = 8.0 * 1024 * 1024 * 1024; 
  std::vector<double> ratios = {0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 0.99, 1.0, 1.01, 1.1, 1.5, 2.0};
  int iterations = 1;

  std::ofstream out("memory_scaling_results.csv");
  out << "C/M,Data_Size_GB,Seq_Time_s,Seq_RSS_KB,Seq_PageFaults,"
          "Rand_Time_s,Rand_RSS_KB,Rand_PageFaults\n";

  for (double ratio : ratios) {
    double data_size_bytes = M_bytes * ratio;
    double data_size_gb = data_size_bytes / (1024.0 * 1024.0 * 1024.0);
    std::cout << "C/M = " << ratio << " (" << data_size_gb << " GB)\n";

    try {
      Metrics seq = run_workload((size_t)data_size_bytes, false, iterations);
      Metrics rnd = run_workload((size_t)data_size_bytes, true, iterations);
      log_results(out, ratio, data_size_gb, seq, rnd);
    } catch (const std::bad_alloc&) {
      std::cerr << "Memory allocation failed at C/M = " << ratio << "\n";
      Metrics empty = {-1, 0, 0};
      log_results(out, ratio, data_size_gb, empty, empty);
    }
  }

  return 0;
}
