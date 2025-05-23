#pragma once
#include "../../ErrorHandler/errorHandler.h"
#include "../../Tokens/tokens.h"
#include <any>
#include <iostream>

// class expressionVisitor;

class expression {
public:
  // expression() {}
  virtual ~expression() = 0;
  virtual void dealloc() = 0;
  virtual void evaluate() = 0;
  virtual std::any getValue() = 0;
  virtual token getErrorLocation() = 0;
  template <typename T> T typeCast(bool &badCast) {
    std::any value = this->getValue();
    if (value.type() != typeid(T)) {
      badCast = true;
      return T();
    }
    return std::any_cast<T>(value);
  }
  virtual bool hadBadCast() = 0;
  // virtual void accept(expressionVisitor &visitor) = 0;

#ifdef DEBUG
  virtual void print() = 0;
#endif
#ifndef DEBUG
  void print() {}
#endif
};
