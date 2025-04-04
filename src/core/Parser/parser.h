#pragma once
#include "Rules/Expressions/binaryExpr.h"
#include "Rules/Expressions/groupingExpr.h"
#include "Rules/Expressions/literalExpr.h"
#include "Rules/Expressions/unaryExpr.h"
#include "Rules/Expressions/variableExpr.h"
#include "Rules/expressions.h"

class parser {

public:
  parser() = default;
  parser(const std::vector<token> &tokens)
      : tokens(std::move(tokens)), pos(0), errorFound(false) {}

  ~parser() { closeFile(); }
  expression *parse();
  bool createFilestream(std::string_view str);
  bool isEmpty() { return pos == tokens.size() - 1; }
  bool isEOF();
  void receiveTokens(const std::vector<token> &token);
  void closeFile();

private:
  expression *declaration();
  expression *expr();
  expression *equality();
  expression *comparison();
  expression *term();
  expression *factor();
  expression *unary();
  expression *primary();
  expression *variableDeclaration();
  token consume(tokenId tk);
  bool check(tokenId token);
  token peek();
  token previous();
  token advance();
  void synchronize();

  void reportError(unsigned errorType);
  void reportError(std::string_view customError);

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
  std::ifstream file;
};
