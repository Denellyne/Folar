#pragma once
#include "../tokens.h"
#include <fstream>
#include <string_view>
#include <vector>

class lexer {
public:
  lexer(std::string_view str);
  ~lexer();

private:
  bool openFile(std::string_view str);
  bool parseFile(std::string_view str);

  tokenId getNextToken();
  char advance();
  char peekAhead();

  unsigned line, column;
  std::ifstream file;
  std::vector<token> errors;
  std::vector<token> tokens;
};
