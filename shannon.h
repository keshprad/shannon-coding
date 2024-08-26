#ifndef SHANNON_H
#define SHANNON_H

#define MAX_ENCODED_SIZE 200

#include "symbol_code.h"
#include "trie.h"
#include "util.h"
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T> class ShannonCode {
private:
  std::vector<T> syms;
  std::unordered_map<T, SymbolCode> sym_codes;
  TrieNode<T> *decode_trie;

  std::vector<std::pair<T, int>> get_frequencies(const std::vector<T> &syms) {
    // create freq map
    std::unordered_map<T, int> freq;
    for (const T &sym : syms)
      freq[sym]++;

    // map to vector
    std::vector<std::pair<T, int>> freq_vec;
    std::copy(freq.begin(), freq.end(), std::back_inserter(freq_vec));

    // sort vector in decreasing order by freq
    std::sort(freq_vec.begin(), freq_vec.end(),
              [](auto const &l, auto const &r) { return l.second > r.second; });
    return freq_vec;
  }
  std::unordered_map<T, SymbolCode>
  assign_codes(const std::vector<std::pair<T, int>> &freq_vec) const {
    // call recursive helper
    std::unordered_map<T, SymbolCode> sym_codes;
    return assign_codes(freq_vec, 0, freq_vec.size() - 1, SymbolCode(),
                        sym_codes);
  }
  std::unordered_map<T, SymbolCode>
  assign_codes(const std::vector<std::pair<T, int>> &freq_vec, int l, int r,
               SymbolCode &&code,
               std::unordered_map<T, SymbolCode> &sym_codes) const {
    // Found code for given symbol
    if (l == r) {
      sym_codes[freq_vec[l].first] = code;
      return sym_codes;
    }

    // Find total and midpt sum
    int total = 0;
    for (int i = l; i <= r; i++) {
      total += freq_vec[i].second;
    }
    // Find index to split on
    int acc = 0, i;
    for (i = l; acc <= total / 2; acc += freq_vec[i++].second)
      ;
    int accprev = acc - freq_vec[i - 1].second;
    if (std::abs(acc - (total - acc)) > std::abs(accprev - (total - accprev))) {
      acc = accprev;
      i--;
    }

    // recurse left
    SymbolCode left_code = SymbolCode((code.code << 1) | 0, code.size + 1);
    assign_codes(freq_vec, l, i - 1, std::move(left_code), sym_codes);

    // recurse right
    SymbolCode right_code = SymbolCode((code.code << 1) | 1, code.size + 1);
    assign_codes(freq_vec, i, r, std::move(right_code), sym_codes);

    return sym_codes;
  }
  TrieNode<T> *build_decode_trie() const {
    TrieNode<T> *root = new TrieNode<T>();

    for (auto p : sym_codes) {
      TrieNode<T> *curr = root;
      SymbolCode &sym_code = p.second;

      // for each bit in code, build new trie node
      for (int shift = sym_code.size - 1; shift >= 0; shift--) {
        int bit = (sym_code.code >> shift) & 1;
        if (!curr->next[bit]) {
          curr->next[bit] = new TrieNode<T>();
        }
        curr = curr->next[bit];
      }
      curr->value = p.first;
    }
    return root;
  }

public:
  ShannonCode(std::vector<T> &syms) : syms(syms) {
    // Get frequency list of each symbol, sorted in decreasing order
    std::vector<std::pair<T, int>> freq_vec = get_frequencies(syms);
    // debug
    std::cout << "freq_vec" << std::endl;
    std::cout << '[';
    for (auto &p : freq_vec) {
      std::cout << '(' << p.first << ", " << p.second << "), ";
    }
    std::cout << ']' << std::endl;

    // Assign codes to each symbol
    sym_codes = assign_codes(freq_vec);
    // debug
    std::cout << "sym_codes" << std::endl;
    std::cout << '{';
    for (auto &p : sym_codes) {
      std::cout << p.first << ": " << p.second << ", ";
    }
    std::cout << "}" << std::endl;

    // Build decode trie
    decode_trie = build_decode_trie();
    // debug
    std::cout << "decode_trie" << std::endl;
    std::cout << decode_trie << std::endl;
  }
  ~ShannonCode() {
    // free entire decode_trie structure
    delete decode_trie;
  }
  std::pair<std::bitset<MAX_ENCODED_SIZE>, size_t> encode_syms() const {
    std::bitset<MAX_ENCODED_SIZE> msg;
    size_t size = 0;

    for (const T &sym : syms) {
      // use at instead of [] to preserve const
      const SymbolCode &sym_code = sym_codes.at(sym);

      // add code to end of message
      msg <<= sym_code.size;
      msg |= sym_code.code;
      size += sym_code.size;
    }

    return {msg, size};
  };
  std::vector<T> decode_syms(const std::bitset<MAX_ENCODED_SIZE> encoded,
                             size_t size) const {
    std::vector<T> decoded;
    TrieNode<T> *curr = decode_trie;

    for (int i = size - 1; i >= 0; i--) {
      curr = curr->next[encoded[i]];
      if (curr->has_value()) {
        decoded.push_back(curr->value);
        curr = decode_trie;
      }
    }

    return decoded;
  };
};
#endif