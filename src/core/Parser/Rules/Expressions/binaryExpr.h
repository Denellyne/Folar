#pragma once
#include "../expressions.h"
#include <utility>

class binaryExpr : public expression {
public:
  binaryExpr(token tk, expression *left, expression *right)
      : tk(tk), left(left), right(right), badCast(false) {}

  // void accept(expressionVisitor &visitor) {}
  virtual void evaluate() {
    left->evaluate();
    right->evaluate();
  }

  virtual token getErrorLocation() {
    if (badCast)
      return tk;
    else if (token errorTk = left->getErrorLocation(); errorTk.id != NOToken)
      return errorTk;
    else if (token errorTk = right->getErrorLocation(); errorTk.id != NOToken)
      return errorTk;
    else
      return token(NOToken, 0, 0, 0);
  }
  virtual std::any getValue() {
    long double leftExpr = left->typeCast<long double>(badCast);
    long double rightExpr = right->typeCast<long double>(badCast);
    if (badCast)
      return nullptr;

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
  bool badCast;
};
