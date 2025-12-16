#pragma once
#include "../Lexer/lexer.h"
#include "../Parser/parser.h"

class interpreter {
public:
  interpreter() = default;
  interpreter(std::string_view str);
  void interpret(std::string_view str);
  ~interpreter();

private:
  void parseFile(std::string_view str);
  void parseTokens();
  void badCastError(std::string_view str, expression *expr);

  lexer lex;
  parser parse;
  std::vector<expression *> expressions;
};
