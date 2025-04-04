#include "compiler.h"

compiler::compiler(std::string_view str) { compile(str); }
compiler::~compiler() {
  lex.closeFile();
  parse.closeFile();
}
void compiler::parseTokens() {

  std::vector<expression *> expressions;

  while (!parse.isEOF()) {
    expression *exprs = parse.parse();
    if (exprs != nullptr)
      expressions.emplace_back(exprs);
  }
#ifdef DEBUG
  std::cout << "Number of expressions: " << expressions.size() << '\n';
#endif
  while (expressions.empty() == false) {
#ifdef DEBUG
    expressions[0]->print();
#endif
    expressions[0]->dealloc();
    expressions.erase(expressions.begin());
  }
}

void compiler::parseFile(std::string_view str) { return compile(str); }
void compiler::compile(std::string_view str) {
  lex.parseFile(str);
  if (errorReport.gotErrors())
    return;

  parse.receiveTokens(lex.getTokens());
  if (!parse.createFilestream(str)) {
    errorReport.reportError(FILEERROR, str);
    return;
  }
  parseTokens();
}
