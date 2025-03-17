#pragma once
#include "keywords.h"
#include "tokenEnum.h"
#include <ostream>

struct token {

  token() = delete;
  token(tokenId id, unsigned line, unsigned column)
      : id(id), line(line), column(column) {}

  token(tokenId id, unsigned line, unsigned column, std::string &literal)
      : id(id), line(line), column(column), literal(std::move(literal)) {}
  void setLiteral(std::string &str) { literal = std::move(str); }

  unsigned line = 0;
  unsigned column = 0;
  tokenId id = NOToken;
  std::string literal = "";
#ifdef DEBUG
  friend std::ostream &operator<<(std::ostream &os, token const &tk) {
    return os << "Token " << tokenNames[tk.id] << " Position " << tk.line << ' '
              << tk.column << " Literal: " << tk.literal;
  }
#endif
};
