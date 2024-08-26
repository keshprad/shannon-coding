#ifndef SYMBOL_CODE_H
#define SYMBOL_CODE_H

#include <cstddef>
#include <iostream>

struct SymbolCode {
  unsigned long long int code;
  size_t size;

  SymbolCode();
  SymbolCode(unsigned long long int code, size_t size);
};

std::ostream &operator<<(std::ostream &os, const SymbolCode &sym_code);

#endif