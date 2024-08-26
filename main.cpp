#include "shannon.h"
#include <iostream>

int main() {
  std::string text = "Happy India Independence Day!!";
  std::vector<char> syms{};
  syms.reserve(text.length());
  for (int i = 0; i < text.length(); i++) {
    syms.emplace_back(text[i]);
  }

  ShannonCode<char> sc(syms);

  // encode message
  std::bitset<MAX_ENCODED_SIZE> encoded;
  size_t encoded_size;
  std::tie(encoded, encoded_size) = sc.encode_syms();
  std::cout << "encoded" << std::endl;
  std::cout << encoded << std::endl;
  std::cout << "encoded_size" << std::endl;
  std::cout << encoded_size << std::endl;

  // decode message
  std::vector<char> decoded = sc.decode_syms(encoded, encoded_size);
  std::cout << "decoded" << std::endl;
  for (char c : decoded) {
    std::cout << c;
  }
  std::cout << std::endl;
}