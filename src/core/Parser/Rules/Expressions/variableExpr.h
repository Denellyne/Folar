#pragma once
#include "../expressions.h"
#include <cassert>

class variableExpr : public expression {
public:
  variableExpr(token tk, expression *expr, std::string_view str,
               bool mut = false)
      : tk(tk), expr(expr), mut(mut) {
    name = new char[str.length() + 1];
    str.copy(name, str.length());
    name[str.length()] = '\0';
  }

  void dealloc() {
    if (expr)
      expr->dealloc();
    expr = nullptr;

    delete[] name;
    name = nullptr;

    delete this;
    return;
  }
  virtual token getErrorLocation() {
    if (expr)
      return expr->getErrorLocation();
    return tk;
  }
  virtual void evaluate() {
    if (expr)
      expr->evaluate();
  }
  virtual std::any getValue() {
    if (mut && expr == nullptr)
      return 0;
    return expr->getValue();
  }

  // virtual void accept(expressionVisitor &visitor) {}
#ifdef DEBUG
  void print() {
    std::cout << name << ' ' << tokenNames[tk.id] << " is mutable: " << mut;
    if (expr != nullptr) {
      std::cout << " Value: ";
      expr->print();
    }
    std::cout << '\n';
  }
#endif

private:
  token tk;
  bool mut;
  char *name;
  expression *expr;
};
