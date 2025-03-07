#include "lexer.h"
#include <iostream>

lexer::lexer() {}

lexer::~lexer() { file.close(); }

bool lexer::parseFile(std::string_view str) {
  if (!openFile(str))
    return false;

  bool errorFound = false;

  tokenId tkId = NOToken;
  do {
    tkId = getNextToken();
    token tk = token(tkId, line, column - 1);

    if (tkId == ERRORToken)
      errorFound = true;
    if (tkId == NEWLineToken) {
      line++;
      column = 0;
    }

  } while (tkId != EOFToken);

  return !errorFound;
}

bool lexer::openFile(std::string_view str) {
  file.open(str.data(), std::fstream::in);
  if (file.fail()) {
    std::cerr << "Failed to open file " << str << " \n";
    return false;
  }
  line = 0;
  column = 0;

  return true;
}

char lexer::advance() {
  char c;
  do {
    column++;
    file.get();
  } while (c == ' ');
  return c;
}

tokenId lexer::getNextToken() {

  switch (advance()) {

  case NEWLineToken:
    return NEWLineToken;
  case EOF:
    return EOFToken;
  default:
    return ERRORToken;
  };

  return ERRORToken;
}
