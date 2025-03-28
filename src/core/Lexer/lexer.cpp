#include "lexer.h"
#include <iostream>

void lexer::closeFile() {
  file.clear();
  file.seekg(0, std::ios::beg);
  file.close();
  while (file.is_open())
    ;
}
bool lexer::parseFile(std::string_view str) {
  if (!openFile(str))
    return false;

  bool errorFound = false;

  tokenId tkId = NOToken;
  do {
    tkId = getNextToken();
    token tk = token(tkId, line, column - 1, filePos);

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
    case CHARLiteralToken:
    case NUMBERLiteralToken:
    case FLOATLiteralToken:
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

#ifndef FUZZER
  std::cout << str << " parsed\n";
#endif
#ifdef DEBUG
  for (const auto &tk : tokens)
    std::cout << tk << '\n';
#endif // DEBUG
  closeFile();
  return !errorFound;
}

bool lexer::openFile(std::string_view str) {
  closeFile();
  file.open(str.data(), std::fstream::in | std::fstream::binary);
  if (file.fail()) {
    closeFile();
    errorReport.reportError(FILEERROR, str);
    return false;
  }
  line = 0;
  column = 0;
  filePos = 0;
  currentLiteral = "";

  return true;
}

[[nodiscard]] char lexer::consume() {
  filePos++;
  column++;
  return file.get();
}

inline char lexer::advance() {
  char c;
  do
    c = consume();
  while (c == ' ' || c == '\t' || c == '\r');
  return c;
}

bool lexer::match(char ch) {
  char c = peekAhead();
  if (c == ch) {
    c = consume();
    return true;
  }
  return false;
}

const char lexer::peekAhead() { return file.peek(); }
const char lexer::peekNextChar() {
  char c;
  do
    c = file.get();
  while (c == ' ');
  file.seekg(filePos);
  return c;
}
bool lexer::handleEscaping() {
  auto isOctal = [](const char c) { return (c >= '0' && c <= '7'); };
  auto isHex = [](const char c) {
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
  };
  auto handleSpecial = [&](auto condition, int length) {
    int i = 0;
    char c = consume();
    if (condition(c) == false)
      return false;
    i += (c - '0');
    bool isOctal = length == 2;
    while (length-- != 0) {
      c = peekAhead();
      if (condition(c) == false)
        break;
      if (isOctal)
        i = (i << 3) + (c - '0');
      else
        i = (i << 4) + (c - '0');
      c = consume();
    }
    currentLiteral += (char)(i);

    return true;
  };

  char c = consume();
  switch (c) {
  case '\'':
    currentLiteral += '\'';
    return true;
  case '"':
    currentLiteral += '\"';
    return true;
  case '?':
    currentLiteral += '\?';
    return true;
  case '\\':
    currentLiteral += '\\';
    return true;
  case 'a':
    currentLiteral += '\a';
    return true;
  case 'b':
    currentLiteral += '\b';
    return true;
  case 'f':
    currentLiteral += '\f';
    return true;
  case 'n':
    currentLiteral += '\n';
    return true;
  case 'r':
    currentLiteral += '\r';
    return true;
  case 't':
    currentLiteral += '\t';
    return true;
  case 'v':
    currentLiteral += '\v';
    return true;
  case 'x':
    return handleSpecial(isHex, 1);
  default:
    file.unget();
    filePos--;
    column--;
    return handleSpecial(isOctal, 2);
  }
  return false;
}
bool lexer::getCharacter() {

  currentLiteral.clear();
  char c;

  while (true) {
    c = consume();
    if (c == '\\') {
      if (!handleEscaping())
        errorReport.reportError(file, line, column, filePos, BADESCAPING);
    } else if (c == '\'')
      return true;
    else if (c == '\n' || c == EOF || c == ';')
      return false;
    else
      currentLiteral += c;
  }
}

bool lexer::getStringLiteral() {
  currentLiteral.clear();
  char c;
  while (true) {
    c = consume();
    if (c == '\\') {
      if (!handleEscaping())
        errorReport.reportError(file, line, column, filePos, BADESCAPING);
    } else if (c == '"')
      return true;
    else if (c == '\n' || c == EOF || c == ';')
      return false;
    else
      currentLiteral += c;
  }
}

int lexer::getNumberLiteral(char ch) {
  auto isValid = [](char c) {
    return (c == ';' || c == ')' || c == ']' || c == '*' || c == '-' ||
            c == '+' || c == '/' || c == '%');
  };
  bool isFloat = false;
  currentLiteral.clear();
  currentLiteral += ch;
  while (true) {
    ch = peekAhead();
    if (ch == '.') {
      ch = consume();
      if (isFloat) {
        errorReport.reportError(file, line, column, filePos, MALFORMEDNUMBER);
        return 0;
      }
      currentLiteral += '.';
      isFloat = true;
    } else if (ch >= '0' && ch <= '9') {
      ch = consume();
      currentLiteral += ch;
    } else if (ch == ' ' && !isValid(peekNextChar())) {
      errorReport.reportError(file, line, column, filePos, MALFORMEDNUMBER);
      return 0;
    } else
      return 1 + isFloat;
  }
}

int lexer::isSpecialCharacter(char c) {
  if (c == '\"')
    return 2;
  else if (c == '\'')
    return 3;
  else if (c == ';' || c == ' ' || c == '+' || c == '=' || c == '-' ||
           c == '*' || c == '/' || c == '<' || c == '>' || c == '(' ||
           c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
           c == '\r' || c == '\t')
    return 1;
  return 0;
};

bool lexer::getSpecialTokens(char ch) {
  auto isAlphaNumeric = [](char c) -> bool {
    if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
      return true;
    return false;
  };

  currentLiteral = ch;
  while (isAlphaNumeric(ch)) {
    switch (isSpecialCharacter(peekAhead())) {
    case 2:
      ch = consume();
      errorReport.reportError(file, line, column, filePos, MALFORMEDSTRING);
      return false;
    case 3:
      ch = consume();
      errorReport.reportError(file, line, column, filePos, MALFORMEDCHAR);
      return false;
    case 1:
      return true;
    }
    ch = consume();
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

  case '*':
    return MULTIPLYToken;

  case '+':
    return ADDToken;
  case '-':
    return SUBToken;
  case '/':
    return DIVIDEToken;
  case '%':
    return MODULUSToken;
  case '{':
    return LBracketToken;
  case '}':
    return RBracketToken;
  case '[':
    return LRectBracketToken;
  case ']':
    return RRectBracketToken;
  case '(':
    return LCurlyBracketToken;
  case ')':
    return RCurlyBracketToken;
  case ':':
    return TYPEIdentifierToken;

  case '\'': {
    if (!getCharacter()) {
      errorReport.reportError(file, line, column, filePos, MALFORMEDCHAR);
      return ERRORToken;
    }
    return CHARLiteralToken;
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
    if (ch >= '0' && ch <= '9') {
      int returnValue = getNumberLiteral(ch);
      if (returnValue == 0)
        return ERRORToken;

      else if (returnValue == 1)
        return NUMBERLiteralToken;

      return FLOATLiteralToken;

    } else if (!getSpecialTokens(ch))
      return ERRORToken;

    if (auto keyword = keywords.find(currentLiteral); keyword != keywords.end())
      return keyword->second;

    return IDENTIFIERToken;
  };

  errorReport.reportError(file, line, column, filePos, ERRORTOKEN);
  return ERRORToken;
}
