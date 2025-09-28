#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <random>

struct HuffmanNode {
  char c;
  int freq;
  HuffmanNode* left;
  HuffmanNode* right;
  HuffmanNode(char c, int freq) : 
    c(c), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
  bool operator()(HuffmanNode* a, HuffmanNode* b) const {
    return a->freq > b->freq;
  }
};

HuffmanNode* buildHuffmanTree(const std::string& data) {
  std::vector<int> freq(256, 0);
  
  for (unsigned char c : data) {
    freq[c]++;
  }

  std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

  for (int i = 0; i < 256; i++) {
    if (freq[i] > 0) {
      pq.push(new HuffmanNode((char)i, freq[i]));
    }
  }

  if (pq.empty()) return nullptr;

  while (pq.size() > 1) {
    HuffmanNode* left = pq.top(); 
    pq.pop();
    HuffmanNode* right = pq.top(); 
    pq.pop();
    HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
  }

  return pq.top();
}

void generateCodes(const HuffmanNode* node, const std::string& path,
                   std::unordered_map<char, std::string>& codeMap) {
  if (!node) return;

  if (!node->left && !node->right) {
    codeMap[node->c] = path.empty() ? "0" : path;
    return;
  }

  generateCodes(node->left, path + "0", codeMap);
  generateCodes(node->right, path + "1", codeMap);
}

std::unordered_map<char, std::string> getCodeMap(const HuffmanNode* root) {
  std::unordered_map<char, std::string> codeMap;
  generateCodes(root, "", codeMap);
  return codeMap;
}

std::string decode(const std::string& encoded, const HuffmanNode* root) {
  if (!root) return "";
  
  std::string decoded;
  
  if (!root->left && !root->right) {
    for (char bit : encoded) {
      decoded += root->c;
    }
    return decoded;
  }
  
  const HuffmanNode* current = root;
  for (char bit : encoded) {
    current = (bit == '0') ? current->left : current->right;
    if (!current->left && !current->right) {
      decoded += current->c;
      current = root;
    }
  }
  return decoded;
}

void deleteTree(HuffmanNode* node) {
  if (!node) return;
  deleteTree(node->left);
  deleteTree(node->right);
  delete node;
}

int main() 
{
  std::string input = "lossless";
  HuffmanNode* root = buildHuffmanTree(input);
  auto codeMap = getCodeMap(root);
  std::string encoded;
  
  for (char c : input) {
    encoded += codeMap[c];
  }

  std::string decoded = decode(encoded, root);
  std::cout << "Original: " << input << "\n";
  std::cout << "Encoded: " << encoded << "\n";
  std::cout << "Decoded: " << decoded << "\n";
  
  deleteTree(root);

  return 0;
}
