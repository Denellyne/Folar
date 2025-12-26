#pragma once

#include "../Ast/ast.h"
#include "../Tokens/tokens.h"
#include <concepts>
#include <fstream>
#include <type_traits>
#include <vector>
template <typename T>
concept tokenIdConcept = (std::same_as<std::common_type_t<T>, tokenId>);

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
  exp *multExp();
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

  constexpr bool matchToType();
  constexpr bool match(const tokenId type);

  template <typename... tokenIdConcept>
    requires(sizeof...(tokenIdConcept) > 1)
  constexpr bool match(const tokenIdConcept... tokens) {
    return (((check(tokens)) ? advance(), true : false) || ...);
  }

  bool errorFound = false;
  unsigned pos = 0;
  std::vector<token> tokens = {};
  std::ifstream file;
};
