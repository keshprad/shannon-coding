#ifndef SHANNON_H
#define SHANNON_H

#define MAX_ENCODED_SIZE 200

#include "trie.h"
#include "util.h"
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T> class ShannonCode {
private:
  std::vector<T> syms;
  std::unordered_map<T, std::string> sym_codes;
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
  std::unordered_map<T, std::string>
  assign_codes(const std::vector<std::pair<T, int>> &freq_vec) const {
    // call recursive helper
    std::unordered_map<T, std::string> sym_codes;
    return assign_codes(freq_vec, 0, freq_vec.size() - 1, "", sym_codes);
  }
  std::unordered_map<T, std::string>
  assign_codes(const std::vector<std::pair<T, int>> &freq_vec, int l, int r,
               std::string code,
               std::unordered_map<T, std::string> &sym_codes) const {
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
    assign_codes(freq_vec, l, i - 1, code + "0", sym_codes);

    // recurse right
    assign_codes(freq_vec, i, r, code + "1", sym_codes);

    return sym_codes;
  }
  TrieNode<T> *build_decode_trie() const {
    TrieNode<T> *root = new TrieNode<T>();

    for (auto p : sym_codes) {
      TrieNode<T> *curr = root;

      // for each char in code, build new trie node
      for (char c : p.second) {
        if (!curr->next[c - '0']) {
          curr->next[c - '0'] = new TrieNode<T>();
        }
        curr = curr->next[c - '0'];
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
      for (auto &ch : sym_codes.at(sym)) {
        msg <<= 1;
        msg |= ch - '0';
        size++;
      }
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