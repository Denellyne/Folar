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
    delete name;
    name = nullptr;
    delete this;
    return;
  }
  virtual void accept(expression *node) {}
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
