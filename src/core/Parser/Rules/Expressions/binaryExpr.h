#pragma once
#include "../expressions.h"
#include <utility>

class binaryExpr : public expression {
public:
  binaryExpr(token tk, expression *left, expression *right)
      : tk(tk), left(left), right(right) {}

  // void accept(expressionVisitor &visitor) {}
  virtual void evaluate() {
    left->evaluate();
    right->evaluate();
  }

  virtual std::any getValue() {
    long double leftExpr = std::any_cast<long double>(left->getValue());
    long double rightExpr = std::any_cast<long double>(right->getValue());
    switch (tk.id) {
    case ADDToken:
      return leftExpr + rightExpr;
    case SUBToken:
      return leftExpr - rightExpr;
    }

    std::unreachable();
  }
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
