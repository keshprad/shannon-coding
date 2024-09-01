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
  EncodedCode encoded = sc.encode_syms();
  std::cout << "encoded code" << std::endl;
  std::cout << encoded.first << std::endl;
  std::cout << "encoded size" << std::endl;
  std::cout << encoded.second << std::endl;

  // decode message
  std::vector<char> decoded = sc.decode_syms(encoded);
  std::cout << "decoded" << std::endl;
  for (char c : decoded) {
    std::cout << c;
  }
  std::cout << std::endl;
}