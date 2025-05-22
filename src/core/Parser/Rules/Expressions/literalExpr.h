#pragma once
#include "../expressions.h"
#include <any>
#include <utility>

class literalExpr : public expression {
private:
  union type {
    char *str;
    long double floatNum;
    long long int intNum;
  };

public:
  void dealloc() {
    if ((terminal == STRINGLiteralToken || terminal == CHARLiteralToken ||
         terminal == IDENTIFIERToken) &&
        literal.str != nullptr) {
      delete[] literal.str;
    }
    literal.str = nullptr;
    delete this;
    return;
  }
  literalExpr(tokenId terminal) : terminal(terminal) {
    switch (terminal) {

    case TRUEToken:
      literal.intNum = 1;
      break;

    case FALSEToken:
      literal.intNum = 0;
      break;
    default:
      literal.intNum = -1;
      break;
    }
  }
  virtual token getErrorLocation() { return token(NOToken, 0, 0, 0); }

  literalExpr(tokenId terminal, std::string_view str) : terminal(terminal) {
    switch (terminal) {
    case FLOATLiteralToken:
      literal.floatNum = std::stold(str.data());
      break;

    case NUMBERLiteralToken:
      literal.intNum = std::atoi(str.data());
      break;

    case IDENTIFIERToken:
    case CHARLiteralToken:
    case STRINGLiteralToken:
      literal.str = new char[str.length() + 1];
      str.copy(literal.str, str.length());
      literal.str[str.length()] = '\0';
      break;

    default:
      terminal = ERRORToken;
      break;
    }
  }

  virtual void evaluate() {}
  virtual std::any getValue() {
    switch (terminal) {
    case FLOATLiteralToken:
      return literal.floatNum;
    case NUMBERLiteralToken:
      return (long double)(literal.intNum);
    case TRUEToken:
    case FALSEToken:
      return (bool)(literal.intNum);
    case NULLToken:
      return nullptr;
    default:
      return literal.str;
    }
    std::unreachable();
  }
  // virtual void accept(expressionVisitor &visitor) {}
#ifdef DEBUG
  void print() {

    switch (terminal) {
    case FLOATLiteralToken:
      std::cout << literal.floatNum;
      break;

    case NUMBERLiteralToken:
      std::cout << literal.intNum;
      break;

    case IDENTIFIERToken:
    case CHARLiteralToken:
    case STRINGLiteralToken:
      std::cout << literal.str;
      break;

    case TRUEToken:
      std::cout << "True";
      break;

    case FALSEToken:
      std::cout << "False";
      break;
    case NULLToken:
      std::cout << "Null";
      break;
    }
  }
#endif

  tokenId terminal;
  type literal;
};
