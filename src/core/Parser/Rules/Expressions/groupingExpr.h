#pragma once
#include "../expressions.h"

class groupingExpr : public expression {
public:
  groupingExpr(expression *expr) : expr(expr) {}

  void dealloc() {
    if (expr != nullptr) {
      expr->dealloc();
      // delete expr;
    }
    expr = nullptr;
    delete this;
    return;
  }

  virtual token getErrorLocation() { return expr->getErrorLocation(); }
  virtual void evaluate() { expr->evaluate(); }
  virtual std::any getValue() { return expr->getValue(); }
  // virtual void accept(expressionVisitor &visitor) {}
#ifdef DEBUG
  void print() {
    if (expr)
      expr->print();
    std::cout << '\n';
  }
#endif

private:
  expression *expr;
};
