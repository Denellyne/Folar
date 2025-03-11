#include "lexer.h"
#include <iostream>

lexer::lexer(std::string_view str) {

  if (!parseFile(str)) {
    std::cerr << "Error found\n";
#ifdef DEBUG

    for (const auto &tk : errors)
      std::cerr << tk << '\n';

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
      errors.emplace_back(tk);
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

  return true;
}

inline char lexer::advance() {
  char c;
  do {
    column++;
    c = file.get();
  } while (c == ' ');
  return c;
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
    char c = peekAhead();
    if (c == '&') {
      advance();
      return ANDToken;
    } else if (c == ' ')
      return BITWISEANDToken;
  } break;
  case '|': {
    char c = peekAhead();
    if (c == '|') {
      advance();
      return ORToken;
    } else if (c == ' ')
      return BITWISEORToken;
  } break;

  case '<': {
    char c = peekAhead();
    if (c == '<') {
      advance();
      return LSHIFTToken;
    } else if (c == '=') {
      advance();
      return LESSEQUALToken;
    } else if (c == ' ')
      return LESSToken;
  } break;

  case '>': {
    char c = peekAhead();
    if (c == '>') {
      advance();
      return RSHIFTToken;
    } else if (c == '=') {
      advance();
      return GREATEQUALToken;
    } else if (c == ' ')
      return GREATToken;
  } break;
  case '!': {
    char c = peekAhead();
    if (c == '=') {
      advance();
      return NOTEQUALToken;
    } else if (c == ' ')
      return NOTToken;
    break;
  }

  default:
    return ERRORToken;
  };

  return ERRORToken;
}
