#pragma once
#include "../expressions.h"

class binaryExpr : public expression {
public:
  binaryExpr(token tk, expression *left, expression *right)
      : tk(tk), left(left), right(right) {}
  void setLeft(expression *node) { left = node; }
  void setRight(expression *node) { right = node; }

  void dealloc() {
    if (left != nullptr)
      left->dealloc();

    if (right != nullptr)
      right->dealloc();

    left = nullptr;
    right = nullptr;
    delete this;
    return;
  }
  virtual void accept(expression *node) {}
#ifdef DEBUG
  void print() {
    std::cout << tokenNames[tk.id] << " L:";
    print(left);
    std::cout << " R:" << print(right);
    std::cout << '\n';
  }
#endif

private:
  token tk;
  expression *left, *right;
};
