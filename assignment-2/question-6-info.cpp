#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <iomanip>
#include <vector>
#include <filesystem>

using namespace std;

void print_memory_status(ofstream& out) {
  struct sysinfo info;
  if (sysinfo(&info) == 0) {
    double total_phys = info.totalram * info.mem_unit / (1024.0 * 1024.0);
    double avail_phys = info.freeram * info.mem_unit / (1024.0 * 1024.0);
    double total_swap = info.totalswap * info.mem_unit / (1024.0 * 1024.0);
    double avail_swap = info.freeswap * info.mem_unit / (1024.0 * 1024.0);

    out << "\nPhysical Memory\n";
    out << "Total Physical Memory: " << total_phys << " MB\n";
    out << "Available Physical Memory: " << avail_phys << " MB\n";
    out << "Total Swap: " << total_swap << " MB\n";
    out << "Available Swap: " << avail_swap << " MB\n";
  }
}

void print_page_size(ofstream& out) {
  long page_size = sysconf(_SC_PAGESIZE);
  long num_proc = sysconf(_SC_NPROCESSORS_ONLN);
  out << "\nSystem Info\n";
  out << "Page Size: " << page_size << " bytes\n";
  out << "Number of Processors: " << num_proc << "\n";
}

void print_cache_info(ofstream& out) {
  out << "\nCache Information\n";
  string base = "/sys/devices/system/cpu/cpu0/cache/";

  for (const auto& entry : filesystem::directory_iterator(base)) {
    string level_path = entry.path().string() + "/level";
    string type_path  = entry.path().string() + "/type";
    string size_path  = entry.path().string() + "/size";

    ifstream level_file(level_path), type_file(type_path), size_file(size_path);
    if (!level_file || !type_file || !size_file) continue;

    string level, type, size;
    getline(level_file, level);
    getline(type_file, type);
    getline(size_file, size);

    out << "Level " << level << " Cache (" << type << ") Size: " << size << "\n";
  }
}

void print_virtual_memory_info(ofstream& out) {
  ifstream file("/proc/meminfo");
  out << "\nVirtual Memory (/proc/meminfo)\n";
  string line;
  int count = 0;
  while (getline(file, line) && count < 10) {
    out << line << "\n";
    ++count;
  }
}

int main() {
  ofstream out("sysinfo_output.txt");
  out << fixed << setprecision(2);

  print_memory_status(out);
  print_page_size(out);
  print_virtual_memory_info(out);
  print_cache_info(out);

  return 0;
}
