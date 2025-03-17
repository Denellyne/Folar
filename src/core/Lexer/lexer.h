#pragma once
#include "../Tokens/tokens.h"
#include <fstream>
#include <string_view>
#include <vector>

class lexer {
public:
  lexer(std::string_view str);
  ~lexer();

  bool parseFile(std::string_view str);

private:
  bool openFile(std::string_view str);

  tokenId getNextToken();
  char advance();
  const char peekAhead();
  bool getStringLiteral();
  bool getCharacter();
  bool getNumberLiteral();
  bool getSpecialTokens(char ch);
  bool match(char ch);

  unsigned line, column, filePos;
  std::ifstream file;
  std::string currentLiteral;
  std::vector<token> tokens;
};
