#pragma once
#include "../../ErrorHandler/errorHandler.h"
#include "../../Tokens/tokens.h"
#include <iostream>

class expression {
public:
  expression() {};
  virtual ~expression();
  virtual void dealloc() = 0;
  virtual void accept(expression *node) = 0;

#ifdef DEBUG
  virtual void print() = 0;
#endif
#ifndef DEBUG
  void print() {}
#endif
};
