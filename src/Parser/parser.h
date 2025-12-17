#pragma once

#include "../Ast/ast.h"
#include "../Tokens/tokens.h"
#include <fstream>
#include <vector>
class parser {

public:
  parser() = default;
  parser(const std::vector<token> &tokens)
      : tokens(std::move(tokens)), pos(0), errorFound(false) {}

  ~parser() { closeFile(); }
  decl *parse();
  bool createFilestream(std::string_view str);
  bool isEmpty() { return pos == tokens.size() - 1; }
  bool isEOF();
  void receiveTokens(const std::vector<token> &token);

private:
  void closeFile();

  decl *declaration();
  func *functionDeclaration();
  stm *structDeclaration();
  stm *statements();
  stm *assignStatement();
  stm *ifStatement();
  stm *whileStatement();
  exp *expr();
  exp *bitwise();
  exp *orExp();
  exp *andExp();
  exp *compExp();
  exp *addExp();
  exp *mulExp();
  exp *powExp();
  exp *unaryExp();
  exp *primary();
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
  bool matchToType() {
    for (const auto &token : tokenTypes) {
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
