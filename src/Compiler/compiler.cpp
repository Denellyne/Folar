#include "compiler.h"

compiler::compiler(std::string_view str) { compile(str); }
compiler::~compiler() {}
void compiler::parseTokens() {

  decl *program = parse.parse();
  if (!program)
    return;
#ifdef DEBUG
  program->printDecl();
#endif
  delete program;
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
