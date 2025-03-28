#pragma once
#include "../expressions.h"

class binaryExpr : public expression {
public:
  binaryExpr(token tk, expression *left, expression *right)
      : tk(tk), left(left), right(right) {}

  void accept(expression *node) {}
  void dealloc() {
    if (left != nullptr) {
      left->dealloc();
      // delete left;
    }
    if (right != nullptr) {
      right->dealloc();
      // delete right;
    }
    left = nullptr;
    right = nullptr;
    delete this;
    return;
  }
#ifdef DEBUG
  void print() {
    std::cout << tokenNames[tk.id] << " L:";
    left->print();
    std::cout << " R:";
    right->print();
    std::cout << '\n';
  }
#endif

private:
  token tk;
  expression *left, *right;
};
