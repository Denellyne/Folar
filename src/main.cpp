#include "core/Lexer/lexer.h"

int main(int argc, char *argv[]) {
  lexer lex("../tests/main.flr");
  lex.parseFile("../tests/ok.flr");
  return 0;
}
