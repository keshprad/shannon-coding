#ifndef TRIE_H
#define TRIE_H
#include <iostream>

template <typename T> struct TrieNode {
  T value;
  // only values are 0 and 1
  TrieNode<T> *next[2] = {nullptr};

  // equivalent to checking for leaf node
  bool has_value() { return !next[0] && !next[1]; };
  void operator delete(void *ptr) {
    if (ptr) {
      TrieNode<T> *node = (TrieNode<T> *)ptr;
      delete node->next[0];
      delete node->next[1];
      std::free(node);
    }
  }
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const TrieNode<T> *node) {
  os << '.' << std::endl;
  print_trie(os, node->next[0], 1, "0");
  print_trie(os, node->next[1], 1, "1");
  return os;
}

template <typename T>
std::ostream &print_trie(std::ostream &os, TrieNode<T> *node, int depth,
                         std::string code) {
  if (!node)
    return os;

  for (int i = 0; i < depth * 4; i++) {
    os << '-';
  }
  if (node->has_value()) {
    os << " " << code[code.length() - 1] << " => (" << code << ", "
       << node->value << ")" << std::endl;
  } else {
    os << " " << code[code.length() - 1] << std::endl;
    print_trie(os, node->next[0], depth + 1, code + "0");
    print_trie(os, node->next[1], depth + 1, code + "1");
  }
  return os;
}

#endif
