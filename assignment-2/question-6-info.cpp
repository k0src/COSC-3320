#include <windows.h>
#include <iostream>
#include <iomanip>
#include <psapi.h>
#include <vector>

void print_memory_status() {
  MEMORYSTATUSEX mem_status = {0};
  mem_status.dwLength = sizeof(mem_status);
  if (GlobalMemoryStatusEx(&mem_status)) {
    std::cout << "\nPhysical Memory\n";
    std::cout << "Total Physical Memory: " << mem_status.ullTotalPhys / (1024 * 1024) << " MB\n";
    std::cout << "Available Physical Memory: " << mem_status.ullAvailPhys / (1024 * 1024) << " MB\n";
    std::cout << "Total Virtual Memory: " << mem_status.ullTotalVirtual / (1024 * 1024) << " MB\n";
    std::cout << "Available Virtual Memory: " << mem_status.ullAvailVirtual / (1024 * 1024) << " MB\n";
  }
}

void print_page_size() {
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);

  std::cout << "\nSystem Info\n";
  std::cout << "Page Size: " << sys_info.dwPageSize << " bytes\n";
  std::cout << "Minimum Application Address: " << sys_info.lpMinimumApplicationAddress << "\n";
  std::cout << "Maximum Application Address: " << sys_info.lpMaximumApplicationAddress << "\n";
  std::cout << "Number of Processors: " << sys_info.dwNumberOfProcessors << "\n";
}

void print_performance_info() {
  PERFORMACE_INFORMATION perf_info = {0};
  perf_info.cb = sizeof(perf_info);

  if (GetPerformanceInfo(&perf_info, sizeof(perf_info))) {
    std::cout << "\nPerformance Information\n";
    std::cout << "System Page Size: " << perf_info.PageSize << " bytes\n";
    std::cout << "Physical Pages: " << perf_info.PhysicalTotal << "\n";
    std::cout << "Available Physical Pages: " << perf_info.PhysicalAvailable << "\n"; 
    std::cout << "System Cache Pages: " << perf_info.SystemCache << "\n";
    std::cout << "Commit Limit: " << perf_info.CommitLimit << "\n";
    std::cout << "Commit Peak: " << perf_info.CommitPeak << "\n";
  }
}

void print_cache_info() {
  DWORD len = 0;
  GetLogicalProcessorInformationEx(RelationCache, nullptr, &len);
  std::vector<char> buffer(len);
  if (!GetLogicalProcessorInformationEx(RelationCache, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data()), &len)) {
    return;
  }

  std::cout << "\nCache Information\n";

  auto* ptr = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data());
  DWORD offset = 0;

  while (offset < len) {
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data() + offset);
    if (info->Relationship == RelationCache) {
      const CACHE_RELATIONSHIP &cache = info->Cache;
      std::cout << "Level: " << static_cast<int>(cache.Level);
      std::cout << " Cache Size: " << cache.CacheSize / 1024 << " KB";
      std::cout << " Line Size: " << cache.LineSize << " bytes";
      std::cout << " Type: " << (cache.Type == CacheData ? "Data" : 
                                 cache.Type == CacheInstruction ? "Instruction" : "Unified") << "\n";
    }
    offset += info->Size;
  }
}

int main()
{
  std::cout << std::fixed << std::setprecision(2);
  print_memory_status();
  print_page_size();
  print_performance_info();
  print_cache_info();

  return 0;
}