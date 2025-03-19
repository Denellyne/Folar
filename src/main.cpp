#include "core/Lexer/lexer.h"
#ifdef FUZZER
#include "fuzz_target.cc"
#endif // FUZZER

int main(int argc, char *argv[]) {
  // lexer lex("../tests/main.flr");
  // lex.parseFile("../tests/ok.flr");
  lexer lex("../tests/ok.flr");
  return 0;
}
