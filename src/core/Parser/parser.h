#pragma once
#include "Rules/Expressions/binaryExpr.h"
#include "Rules/Expressions/groupingExpr.h"
#include "Rules/Expressions/literalExpr.h"
#include "Rules/Expressions/unaryExpr.h"
#include "Rules/expressions.h"

class parser {

public:
  parser(const std::vector<token> &tokens)
      : tokens(std::move(tokens)), pos(0), errorFound(false) {}

  expression *parse();
  bool isEmpty() { return pos == tokens.size() - 1; }
  bool isEOF();

private:
  expression *expr();
  expression *equality();
  expression *comparison();
  expression *term();
  expression *factor();
  expression *unary();
  expression *primary();
  token consume(tokenId tk);
  bool check(tokenId token);
  token peek();
  token previous();
  token advance();
  void synchronize();

  template <typename... tokenId> bool match(tokenId... types) {
    for (const auto &token : {types...}) {
      if (check(token)) {
        advance();
        return true;
      }
    }
    return false;
  }

  bool errorFound = false;
  unsigned pos = 0;
  std::vector<token> tokens = {};
};
