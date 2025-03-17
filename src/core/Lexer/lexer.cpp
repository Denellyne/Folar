#include "lexer.h"
#include "../ErrorHandler/errorHandler.h"
#include <iostream>

lexer::lexer(std::string_view str) {

  if (!parseFile(str))
    std::cerr << "Error found\n";

#ifdef DEBUG
  for (const auto &tk : tokens)
    std::cout << tk << '\n';
#endif // DEBUG
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

    switch (tkId) {
    case ERRORToken:
      errorFound = true;
      continue;
      break;
    case NEWLineToken:
      line++;
      column = 0;
      break;
    case STRINGLiteralToken:
    case IDENTIFIERToken:
    case INT8Token:
    case INT16Token:
    case INT32Token:
    case INT64Token:
    case UInt8Token:
    case UInt16Token:
    case UInt32Token:
    case UInt64Token:
    case FLOAT32Token:
    case FLOAT64Token:
    case FLOAT128Token:
      tk.setLiteral(currentLiteral);
      break;
    }

    tokens.emplace_back(tk);

  } while (tkId != EOFToken);

  std::cout << str << " parsed\n";
  file.close();
  return !errorFound;
}

bool lexer::openFile(std::string_view str) {
  file.open(str.data(), std::fstream::in);
  if (file.fail()) {
    errorReport.reportError(FILEERROR, str);
    file.close();
    return false;
  }
  line = 0;
  column = 0;
  filePos = 0;
  currentLiteral = "";

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

const char lexer::peekAhead() { return file.peek(); }

bool lexer::getStringLiteral() {
  currentLiteral.clear();
  char c;
  while (true) {
    c = file.get();
    column++;
    filePos++;
    if (c == '\"')
      return true;
    if (c == ';' || c == '\n' || c == EOF)
      return false;
    currentLiteral += c;
  }
}

bool lexer::getSpecialTokens(char ch) {
  auto isAlphaNumeric = [](char c) -> bool {
    if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
      return true;
    return false;
  };
  auto isSpecialCharacter = [](char c) {
    if (c == '\"' || c == '\'')
      return 2;
    if (c == ';' || c == ' ' || c == '+' || c == '=' || c == '-' || c == '*' ||
        c == '/' || c == '<' || c == '>' || c == '(' || c == ')' || c == '[' ||
        c == ']' || c == '{' || c == '}' || c == '\r' || c == '\t')
      return 1;
    return 0;
  };

  currentLiteral = ch;
  while (isAlphaNumeric(ch)) {
    switch (isSpecialCharacter(peekAhead())) {
    case 2:
      filePos++;
      column++;
      file.get();
      errorReport.reportError(file, line, column, filePos, MALFORMEDSTRING);
      return false;
    case 1:
      return true;
    }
    ch = file.get();
    column++;
    filePos++;
    currentLiteral += ch;
  }
  errorReport.reportError(file, line, column, filePos, ERRORTOKEN);
  return false;
}

tokenId lexer::getNextToken() {

  switch (char ch = advance()) {

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
  case '=': {
    if (match('='))
      return EQUALToken;
    return ASSIGNToken;
  }

  case '*': {
    break;
  }
  case '\"': {
    if (!getStringLiteral()) {
      errorReport.reportError(file, line, column, filePos, MALFORMEDSTRING);
      return ERRORToken;
    }
    return STRINGLiteralToken;
  }
  case '!': {
    if (match('='))
      return NOTEQUALToken;
    return NOTToken;
  }
  case ';':
    return ENDStatementToken;

  default:
    if (!getSpecialTokens(ch))
      return ERRORToken;

    if (auto keyword = keywords.find(currentLiteral); keyword != keywords.end())
      return keyword->second;

    return IDENTIFIERToken;
  };

  errorReport.reportError(file, line, column, filePos, ERRORTOKEN);
  return ERRORToken;
}
