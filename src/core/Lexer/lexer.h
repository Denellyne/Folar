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
  std::vector<token> &getTokens() { return tokens; }
#ifdef FUZZER
  lexer(std::string_view str, bool &errorFound);
#endif

private:
  bool openFile(std::string_view str);

  tokenId getNextToken();
  char advance();
  const char peekAhead();
  const char peekNextChar();
  [[nodiscard]] char consume();
  bool getStringLiteral();
  bool getCharacter();
  bool handleEscaping();
  int getNumberLiteral(char ch);
  bool getSpecialTokens(char ch);
  int isSpecialCharacter(char ch);
  bool match(char ch);

  unsigned line, column, filePos;
  std::ifstream file;
  std::string currentLiteral;
  std::vector<token> tokens;
};
