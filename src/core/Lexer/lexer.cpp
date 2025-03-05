#include "lexer.h"
#include <iostream>

lexer::lexer() {}

lexer::~lexer() { file.close(); }

bool lexer::parseFile(std::string_view str) {
  if (!openFile(str))
    return false;

  bool errorFound = false;

  tokenId tkId = NOToken;
  unsigned line = 0, column = 0;
  do {
    tkId = getNextToken();
    token tk = token(tkId, line, column);

    if (tkId == ERRORToken)
      errorFound = true;

  } while (tkId != EOFToken);

  return !errorFound;
}

bool lexer::openFile(std::string_view str) {
  file.open(str.data(), std::fstream::in);
  if (file.fail()) {
    std::cerr << "Failed to open file " << str << " \n";
    return false;
  }

  return true;
}

tokenId lexer::getNextToken() {
  char c = file.get();

  switch (c) {

  case EOF:
    return EOFToken;
  default:
    return ERRORToken;
  };

  return ERRORToken;
}
