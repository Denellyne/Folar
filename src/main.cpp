#include "core/Lexer/lexer.h"

int main(int argc, char *argv[]) {
  lexer lex;
  lex.parseFile("../src/main.cpp");
  return 0;
}
