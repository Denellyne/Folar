#pragma once
#include "../expressions.h"
#include <cassert>
#include <utility>

class unaryExpr : public expression {
public:
  unaryExpr(token tk, expression *expr) : tk(tk), expr(expr), badCast(false) {}

  void dealloc() {
    if (expr) {
      expr->dealloc();
    }

    expr = nullptr;
    delete this;
    return;
  }
  // virtual void accept(expressionVisitor &visitor) {}
  virtual token getErrorLocation() {
    if (badCast)
      return tk;
    else if (token errorTk = expr->getErrorLocation(); errorTk.id != NOToken)
      return errorTk;
    else
      return token(NOToken, 0, 0, 0);
  }
  virtual void evaluate() { expr->evaluate(); }
  virtual std::any getValue() {
    long double right = expr->typeCast<long double>(badCast);
    if (badCast)
      return nullptr;

    if (tk.id == SUBToken) {
      return -right;
    } else
      return !right;

    std::unreachable();
  }
#ifdef DEBUG
  void print() {
    std::cout << tokenNames[tk.id] << " L:";
    expr->print();
    std::cout << '\n';
  }
#endif

private:
  token tk;
  expression *expr;
  bool badCast;
};
