#pragma once
#include "../expressions.h"

class unaryExpr : public expression {
public:
  unaryExpr(token tk, expression *left) : tk(tk) {}

  virtual void accept(expression *node) {}
#ifdef DEBUG
  void print() {
    std::cout << tokenNames[tk.id] << " L:";
    left->print();
    std::cout << '\n';
  }
#endif

private:
  token tk;
  expression *left;
};
