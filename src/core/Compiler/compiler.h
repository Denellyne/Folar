#pragma once
#include "../Lexer/lexer.h"
#include "../Parser/parser.h"

class compiler {
public:
  compiler() = default;
  compiler(std::string_view str);
  void compile(std::string_view str);
  ~compiler();

private:
  void parseFile(std::string_view str);
  void parseTokens();

  lexer lex;
  parser parse;
};
