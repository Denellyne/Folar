#pragma once
#include "../ErrorHandler/errorHandler.h"
#include "../Lexer/lexer.h"
#include "../Parser/parser.h"
#include <fstream>

class compiler {
public:
  compiler() = default;
  compiler(std::string_view str);
  ~compiler();

private:
  void parseFile(std::string_view str);
  void parseTokens();
  void compile(std::string_view str);

  lexer lex;
  parser parse;
  errorHandler errorReport;
};
