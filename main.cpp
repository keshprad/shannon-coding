#include "shannon.h"
#include <boost/dynamic_bitset.hpp>
#include <iostream>

int main() {
  // specify input text
  std::string input_text = "Happy India Independence Day!!";
  std::cout << "input_text: " << input_text << std::endl
            << "input_text bits: "
            << input_text.length() * (sizeof(char) * 1 << 3) << std::endl;

  // create vector of symbols
  std::vector<char> syms{};
  syms.reserve(input_text.length());
  for (int i = 0; i < input_text.length(); i++) {
    syms.emplace_back(input_text[i]);
  }

  // construct ShannonCode object
  ShannonCode<char> sc(syms);

  // encode message
  boost::dynamic_bitset<> encoded = sc.encode();
  std::cout << "encoded: " << encoded << std::endl
            << "encoded.size(): " << encoded.size() << std::endl;

  // decode message
  std::vector<char> decoded_vec = sc.decode(encoded);
  std::string decoded{decoded_vec.begin(), decoded_vec.end()};
  std::cout << "decoded: " << decoded << std::endl;
}