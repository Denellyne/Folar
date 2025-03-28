#include "compiler.h"

compiler::compiler(std::string_view str) { compile(str); }
compiler::~compiler() {
  lex.closeFile();
  parse.closeFile();
}
void compiler::parseTokens() {

  while (!parse.isEOF()) {
    expression *exprs = parse.parse();
    if (exprs != nullptr) {
      exprs->print();
      exprs->dealloc();
      // delete exprs;
    }
  }
}

void compiler::parseFile(std::string_view str) { return compile(str); }
void compiler::compile(std::string_view str) {
  lex.parseFile(str);
  parse.receiveTokens(lex.getTokens());
  if (!parse.createFilestream(str)) {
    errorReport.reportError(FILEERROR, str);
    return;
  }
  parseTokens();
  parse.closeFile();
}
