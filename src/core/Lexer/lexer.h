#pragma once
#include "../tokens.h"
#include <fstream>
#include <string_view>

class lexer {
public:
  lexer();
  ~lexer();
  bool parseFile(std::string_view str);

private:
  bool openFile(std::string_view str);

  tokenId getNextToken();
  char advance();
  char peekAhead();

  unsigned line, column;
  std::ifstream file;
};
