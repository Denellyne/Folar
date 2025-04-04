#pragma once
#include "../expressions.h"
#include <cassert>

class declarationExpr : public expression {
public:
  declarationExpr(expression *expr) : expr(expr) {}

  void dealloc() {
    if (expr)
      expr->dealloc();

    expr = nullptr;
    delete this;
    return;
  }
  virtual void accept(expression *node) {}
#ifdef DEBUG
  void print() { expr->print(); }
#endif

private:
  expression *expr;
};
