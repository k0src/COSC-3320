#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <chrono>
#include <bitset>
#include <cmath>

// int symbols for testing
struct HuffmanNode {
  int sym;
  uint64_t freq;
  HuffmanNode* left;
  HuffmanNode* right;
  HuffmanNode(int sym, uint64_t freq) : 
    sym(sym), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
  bool operator()(HuffmanNode* a, HuffmanNode* b) const {
    return a->freq > b->freq;
  }
};

HuffmanNode* buildHuffmanTree(const std::vector<int>& data) {
  std::unordered_map<int, uint64_t> freq;
  
  for (int sym : data) {
    freq[sym]++;
  }

  std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

  for (const auto& [sym, f] : freq) {
    pq.push(new HuffmanNode(sym, f));
  }

  if (pq.empty()) return nullptr;

  while (pq.size() > 1) {
    HuffmanNode* left = pq.top(); 
    pq.pop();
    HuffmanNode* right = pq.top(); 
    pq.pop();
    HuffmanNode* parent = new HuffmanNode(-1, left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
  }

  return pq.top();
}

void generateCodes(const HuffmanNode* node, const std::string& path,
                   std::unordered_map<int, std::string>& codeMap) {
  if (!node) return;

  if (!node->left && !node->right) {
    codeMap[node->sym] = path.empty() ? "0" : path; 
    return;
  }

  generateCodes(node->left, path + "0", codeMap);
  generateCodes(node->right, path + "1", codeMap);
}

std::unordered_map<int, std::string> getCodeMap(const HuffmanNode* root) {
  std::unordered_map<int, std::string> codeMap;
  generateCodes(root, "", codeMap);
  return codeMap;
}

std::vector<uint8_t> encode(const std::vector<int>& data,
                            const std::unordered_map<int, std::string>& codeMap) {
  std::vector<uint8_t> encoded;
  uint8_t currentByte = 0;
  int bitPos = 0;

  for (int sym : data) {
    const std::string& code = codeMap.at(sym);
    for (char bit : code) {
      if (bit == '1') {
        currentByte |= (1 << (7 - bitPos));
      }
      bitPos++;
      if (bitPos == 8) {
        encoded.push_back(currentByte);
        currentByte = 0;
        bitPos = 0;
      }
    }
  }

  if (bitPos > 0) {
    encoded.push_back(currentByte);
  }

  return encoded;
}

std::vector<int> decode(const std::vector<uint8_t>& encoded, const HuffmanNode* root, size_t originalSize) {
  if (!root) return {};

  std::vector<int> decoded;
  decoded.reserve(originalSize);

  if (!root->left && !root->right) {
    decoded.resize(originalSize, root->sym);
    return decoded;
  }

  const HuffmanNode* current = root;
  for (uint8_t byte : encoded) {
    for (int i = 7; i >= 0 && decoded.size() < originalSize; i--) {
      bool bit = (byte >> i) & 1;
      current = bit ? current->right : current->left;
      if (!current->left && !current->right) {
        decoded.push_back(current->sym);
        current = root;
      }
    }
    if (decoded.size() >= originalSize) break;
  }

  return decoded;
}

void deleteTree(HuffmanNode* node) {
  if (!node) return;
  deleteTree(node->left);
  deleteTree(node->right);
  delete node;
}

// generate n random symbols with values in range [0, sigma-1]
std::vector<int> generateSymbols(int n, int sigma) {
  std::vector<int> data;
  data.reserve(n);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, sigma - 1);

  for (int i = 0; i < n; i++) {
    data.push_back(dist(gen));
  }

  return data;
}

std::pair<double, double> getAverageExecutionTime(int n, int sigma, int numberOfExecutions) {
  double totalEncodeTime = 0.0;
  double totalDecodeTime = 0.0;

  for (int i = 0; i < numberOfExecutions; i++) {
    std::vector<int> data = generateSymbols(n, sigma);

    // Encode 
    auto start = std::chrono::high_resolution_clock::now();
    HuffmanNode* root = buildHuffmanTree(data);
    auto codeMap = getCodeMap(root);
    std::vector<uint8_t> encoded = encode(data, codeMap);
    auto end = std::chrono::high_resolution_clock::now();
    totalEncodeTime += std::chrono::duration<double, std::milli>(end - start).count();

    // Decode
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> decoded = decode(encoded, root, data.size());
    end = std::chrono::high_resolution_clock::now();
    totalDecodeTime += std::chrono::duration<double, std::milli>(end - start).count();

    deleteTree(root);
  }

  return {totalEncodeTime / numberOfExecutions, totalDecodeTime / numberOfExecutions};
}

int main() 
{
  int numberOfExecutions = 1;
  std::vector<int> exponents = {10, 12, 14, 16, 18}; // n = 2^exponent

  std::cout << "sigma = 256\n";
  for (int i = 0; i < exponents.size(); i++) {
    int n = std::pow(2, exponents[i]);
    int sigma = 256;

    auto [avgEncodeTime, avgDecodeTime] = getAverageExecutionTime(n, sigma, numberOfExecutions);
    std::cout << "n = 2^" << exponents[i] << " (" << n << "), "
              << "sigma = " << sigma << ": "
              << "Avg Encode Time = " << avgEncodeTime << " ms, "
              << "Avg Decode Time = " << avgDecodeTime << " ms\n";
  }

  std::cout << std::endl;

  std::cout << "sigma = sqrt(n)\n";
  for (int i = 0; i < exponents.size(); i++) {
    int n = std::pow(2, exponents[i]);
    int sigma = std::sqrt(n);

    auto [avgEncodeTime, avgDecodeTime] = getAverageExecutionTime(n, sigma, numberOfExecutions);
    std::cout << "n = 2^" << exponents[i] << " (" << n << "), "
              << "sigma = sqrt(n) = " << sigma << ": "
              << "Avg Encode Time = " << avgEncodeTime << " ms, "
              << "Avg Decode Time = " << avgDecodeTime << " ms\n";
  }

  std::cout << std::endl;

  std::cout << "sigma = n/10\n";
  for (int i = 0; i < exponents.size(); i++) {
    int n = std::pow(2, exponents[i]);
    int sigma = n / 10;

    auto [avgEncodeTime, avgDecodeTime] = getAverageExecutionTime(n, sigma, numberOfExecutions);
    std::cout << "n = 2^" << exponents[i] << " (" << n << "), "
              << "sigma = n/10 = " << sigma << ": "
              << "Avg Encode Time = " << avgEncodeTime << " ms, "
              << "Avg Decode Time = " << avgDecodeTime << " ms\n";
  }

  std::cout << std::endl;

  std::cout << "sigma = n\n";
  for (int i = 0; i < exponents.size(); i++) {
    int n = std::pow(2, exponents[i]);
    int sigma = n;

    auto [avgEncodeTime, avgDecodeTime] = getAverageExecutionTime(n, sigma, numberOfExecutions);
    std::cout << "n = 2^" << exponents[i] << " (" << n << "), "
              << "sigma = n = " << sigma << ": "
              << "Avg Encode Time = " << avgEncodeTime << " ms, "
              << "Avg Decode Time = " << avgDecodeTime << " ms\n";
  }

  std::cout << std::endl;

  return 0;
}



