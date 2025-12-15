#include "compiler.h"

compiler::compiler(std::string_view str) { compile(str); }
compiler::~compiler() {
  lex.closeFile();
  parse.closeFile();
}
void compiler::parseTokens() {

  stm *stms = parse.parse();
  if (!stms)
    return;
#ifdef DEBUG
  printStm(stms);
#endif
  delete stms;
}

void compiler::parseFile(std::string_view str) { return compile(str); }
void compiler::compile(std::string_view str) {
  lex.parseFile(str);
  if (errorHandler::getInstance().gotErrors())
    return;

  parse.receiveTokens(lex.getTokens());
  if (!parse.createFilestream(str)) {
    errorHandler::getInstance().reportError(FILEERROR, str);
    return;
  }
  parseTokens();
}
