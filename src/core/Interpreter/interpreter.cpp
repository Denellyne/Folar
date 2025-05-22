#include "interpreter.h"
#include <any>
#include <typeindex>

interpreter::interpreter(std::string_view str) { interpret(str); }
interpreter::~interpreter() {
  lex.closeFile();
  parse.closeFile();
  while (expressions.empty() == false) {
#ifdef DEBUG
    expressions[0]->print();
#endif
    expressions[0]->dealloc();
    expressions.erase(expressions.begin());
  }
}
void interpreter::parseTokens() {

  while (!parse.isEOF()) {
    expression *exprs = parse.parse();

    if (exprs != nullptr)
      expressions.emplace_back(exprs);

    if (errorReport.gotErrors()) {
      while (expressions.empty() == false) {
        expressions[0]->dealloc();
        expressions.erase(expressions.begin());
      }
      return;
    }
  }

  // for (const auto &expr : expressions)
  //   expr->accept();

#ifdef DEBUG
  std::cout << "Number of expressions: " << expressions.size() << '\n';
#endif
}

void interpreter::parseFile(std::string_view str) { return interpret(str); }
void interpreter::interpret(std::string_view str) {
  lex.parseFile(str);
  if (errorReport.gotErrors())
    return;

  parse.receiveTokens(lex.getTokens());
  if (!parse.createFilestream(str)) {
    errorReport.reportError(FILEERROR, str);
    return;
  }
  parseTokens();
  if (errorReport.gotErrors())
    return;
  for (auto &expr : expressions) {
    std::any value = expr->getValue();
    if (value.type() == typeid(long double))
      std::cout << std::any_cast<long double>(value) << '\n';

    else if (value.type() == typeid(char *))
      std::cout << std::any_cast<char *>(value) << '\n';
  }
}
