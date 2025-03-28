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

  void accept(expression *node) {}
#ifdef DEBUG
  void print() {
    expr->print();
    std::cout << '\n';
  }
#endif

private:
  expression *expr;
};
