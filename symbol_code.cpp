#include "symbol_code.h"
#include <cstddef>

SymbolCode::SymbolCode() : code(0), size(0) {}
SymbolCode::SymbolCode(unsigned long long int code, size_t size)
    : code(code), size(size) {}

std::ostream &operator<<(std::ostream &os, const SymbolCode &sym_code) {
  for (int shift = sym_code.size - 1; shift >= 0; shift--) {
    os << ((sym_code.code >> shift) & 1);
  }
  return os;
}