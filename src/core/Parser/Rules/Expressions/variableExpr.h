#pragma once
#include "../expressions.h"
#include "literalExpr.h"
#include <cassert>

class variableExpr : public expression {
public:
  variableExpr(token tk, expression *expr, std::string_view str,
               bool mut = false)
      : tk(tk), expr(expr), mut(mut), badCast(false) {
    name = new char[str.length() + 1];
    str.copy(name, str.length());
    name[str.length()] = '\0';
  }

  void dealloc() {
    if (expr)
      expr->dealloc();
    expr = nullptr;

    delete[] name;
    name = nullptr;

    delete this;
    return;
  }

  virtual bool hadBadCast() { return badCast; }
  virtual token getErrorLocation() {
    if (badCast)
      return tk;
    if (expr)
      return expr->getErrorLocation();
    return tk;
  }
  virtual void evaluate() {
    if (expr)
      expr->evaluate();
  }
  virtual std::any getValue() {
    if (mut && expr == nullptr)
      return 0;

    switch (tk.id) {
    case FLOAT32Token:
    case FLOAT64Token:
    case FLOAT128Token:

      return expr->typeCast<long double>(badCast);
    case INT8Token:
    case INT16Token:
    case INT32Token:
    case INT64Token:

    case UInt8Token:
    case UInt16Token:
    case UInt32Token:
    case UInt64Token:
      return expr->typeCast<long double>(badCast);
    case BOOLToken:
      return expr->typeCast<bool>(badCast);
    case NULLToken:
      return nullptr;

    case IDENTIFIERToken:
    case STRINGToken:
    case CHARToken:

      return expr->typeCast<char *>(badCast);
    default:
      badCast = true;
      return nullptr;
    }
    std::unreachable();
  }

  // virtual void accept(expressionVisitor &visitor) {}
#ifdef DEBUG
  void print() {
    std::cout << name << ' ' << tokenNames[tk.id] << " is mutable: " << mut;
    if (expr != nullptr) {
      std::cout << " Value: ";
      expr->print();
    }
    std::cout << '\n';
  }
#endif

private:
  token tk;
  bool mut;
  bool badCast;
  char *name;
  expression *expr;
};
