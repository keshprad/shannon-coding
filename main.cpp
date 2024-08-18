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
  std::string encoded = sc.encode_syms();
  std::cout << "encoded" << std::endl;
  std::cout << encoded << std::endl;

  std::vector<char> decoded = sc.decode_syms(encoded);
  std::cout << "decoded" << std::endl;
  for (char c : decoded) {
    std::cout << c;
  }
  std::cout << std::endl;
}