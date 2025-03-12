#include "lexer.h"
#include "../ErrorHandler/errorHandler.h"
#include <iostream>

lexer::lexer(std::string_view str) {

  if (!parseFile(str)) {
    std::cerr << "Error found\n";
#ifdef DEBUG

    for (const auto &tk : tokens)
      std::cout << tk << '\n';
#endif // DEBUG
  }
}

lexer::~lexer() { file.close(); }

bool lexer::parseFile(std::string_view str) {
  if (!openFile(str))
    return false;

  bool errorFound = false;

  tokenId tkId = NOToken;
  do {
    tkId = getNextToken();
    token tk = token(tkId, line, column - 1);

    if (tkId == ERRORToken) {
      errorFound = true;
    } else
      tokens.emplace_back(tk);

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
  filePos = 0;

  return true;
}

inline char lexer::advance() {
  char c;
  do {
    column++;
    filePos++;
    c = file.get();
  } while (c == ' ' || c == '\t' || c == '\r');
  return c;
}

bool lexer::match(char ch) {
  char c = peekAhead();
  if (c == ch) {
    file.get();
    column++;
    filePos++;
    return true;
  }
  return false;
}

char lexer::peekAhead() { return file.peek(); }

tokenId lexer::getNextToken() {

  switch (advance()) {

  case '\n':
    return NEWLineToken;
  case EOF:
    return EOFToken;
  case '~':
    return BITWISENOTToken;
  case '&': {
    if (match('&'))
      return ANDToken;

    return BITWISEANDToken;
  }
  case '|': {
    if (match('|'))
      return ORToken;

    return BITWISEORToken;
  }

  case '<': {
    if (match('<'))
      return LSHIFTToken;
    else if (match('='))
      return LESSEQUALToken;
    return LESSToken;
  }

  case '>': {
    if (match('>'))
      return RSHIFTToken;
    else if (match('='))
      return GREATEQUALToken;
    return GREATToken;
  }
  case '!': {
    if (match('='))
      return NOTEQUALToken;
    return NOTToken;
  }

  default:
    errorReport.reportError(file, line, column, filePos, ERRORTOKEN);
    return ERRORToken;
  };

  errorReport.reportError(file, line, column, filePos, ERRORTOKEN);
  return ERRORToken;
}
