#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <executions>" << std::endl;
    return 1;
  }

  int executions;
  try {
    executions = std::stoi(argv[1]);
    if (executions <= 0) throw std::invalid_argument("Non-positive number");
  } catch (const std::exception&) {
    std::cerr << "Invalid arguments." << std::endl;
    return 1;
  }

  std::cout << "\nRunning Binary Search Performance Tests" << std::endl;
  std::cout << "Executions per test: " << executions << std::endl << std::endl;

  std::vector<std::string> commands = {
    "./tests/test " + std::to_string(executions),           
    "python3 tests/test.py " + std::to_string(executions),
    "node tests/test.js " + std::to_string(executions)
  };

  std::vector<std::string> test_names = {
    "C++ Test",
    "Python Test", 
    "TypeScript Test"
  };

  for (size_t i = 0; i < commands.size(); i++) {
    std::cout << "Running " << test_names[i] << "..." << std::endl;
    int result = std::system(commands[i].c_str());
    if (result != 0) {
      std::cerr << test_names[i] << " failed with exit code " << result << std::endl;
      return 1;
    }
    std::cout << std::endl;
  }

  return 0;
}