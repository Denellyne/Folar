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
  virtual std::any getValue() { return expr->getValue(); }
  virtual token getErrorLocation() { return expr->getErrorLocation(); }
  virtual void evaluate() { expr->evaluate(); }
  // virtual void accept(expressionVisitor &visitor) {}
#ifdef DEBUG
  void print() { expr->print(); }
#endif

private:
  expression *expr;
};
