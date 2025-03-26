#include "core/Lexer/lexer.h"
#include "core/Parser/parser.h"
#include "core/Tokens/tokenEnum.h"
#ifdef FUZZER
#include "fuzz_target.cc"
#endif // FUZZER

int main(int argc, char *argv[]) {
  // lexer lex("../tests/main.flr");
  // lex.parseFile("../tests/ok.flr");
  // lexer lex("../tests/ok.flr");
  lexer lex("../tests/a.flr");
  parser parse(lex.getTokens());
  while (!parse.isEOF()) {
    expression *exprs = parse.parse();
    if (exprs != nullptr)
      exprs->print();
  }
  return 0;
}
