#include "core/Compiler/compiler.h"
#ifdef FUZZER
#include "fuzz_target.cc"
#endif // FUZZER

int main(int argc, char *argv[]) {
  // lexer lex("../tests/main.flr");
  // lex.parseFile("../tests/ok.flr");
  // lexer lex("../tests/ok.flr");
  if (argc > 1) {
    compiler folar(argv[1]);
  } else
    compiler folar("../tests/a.flr");
  return 0;
}
