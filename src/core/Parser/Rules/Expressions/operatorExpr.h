#pragma once
#include "../expressions.h"

class binaryExpr : public expression {
public:
  binaryExpr(token tk) : tk(tk) {}
  void setLeft(expression *node) { left = node; }
  void setRight(expression *node) { right = node; }

  virtual void accept(expression *node) {}
#ifdef DEBUG
  void print() {
    std::cout << tokenNames[tk.id] << " L:" << print(left)
              << " R:" << print(right) << '\n';
  }
#endif

private:
  token tk;
  expression *left, *right;
};
