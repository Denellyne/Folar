#pragma once
#include "../expressions.h"
#include <cassert>

class unaryExpr : public expression {
public:
  unaryExpr(token tk, expression *expr) : tk(tk), expr(expr) {}

  void dealloc() {
    if (expr) {
      expr->dealloc();
      // delete expr;
    }

    expr = nullptr;
    delete this;
    return;
  }
  virtual void accept(expression *node) {}
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
