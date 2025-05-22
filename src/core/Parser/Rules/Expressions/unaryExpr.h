#pragma once
#include "../expressions.h"
#include <cassert>
#include <utility>

class unaryExpr : public expression {
public:
  unaryExpr(token tk, expression *expr) : tk(tk), expr(expr) {}

  void dealloc() {
    if (expr) {
      expr->dealloc();
    }

    expr = nullptr;
    delete this;
    return;
  }
  // virtual void accept(expressionVisitor &visitor) {}
  virtual void evaluate() { expr->evaluate(); }
  virtual std::any getValue() {
    long double right = std::any_cast<long double>(expr->getValue());
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
};
