#include "parser.h"
#include "Rules/Expressions/literalExpr.h"
#include "Rules/expressions.h"
bool parser::createFilestream(std::string_view str) {
  closeFile();
  file.open(str.data(), std::fstream::in | std::fstream::binary);
  if (file.fail()) {
    closeFile();
    errorReport.reportError(FILEERROR, str);
    return false;
  }
  return true;
}
void parser::closeFile() {
  file.clear();
  file.seekg(0, std::ios::beg);
  file.close();
  while (file.is_open())
    ;
}
void parser::receiveTokens(const std::vector<token> &token) {
  tokens = std::move(token);
  pos = 0;
  errorFound = 0;
}
bool parser::isEOF() { return peek().id == EOFToken; }
token parser::peek() { return tokens[pos]; }
token parser::previous() { return tokens[pos - 1]; }
token parser::advance() {
  if (!isEOF())
    pos++;
  return previous();
}
bool parser::check(tokenId token) {
  if (isEOF())
    return false;
  return peek().id == token;
}

token parser::consume(tokenId tk) {
  if (check(tk))
    return advance();

  std::string str;
  switch (tk) {
  case RCurlyBracketToken:
    str = tokenNames[peek().id];
    str += " should had been" + tokenNames[RCurlyBracketToken];
    break;
  }

  errorFound = true;
  errorReport.reportError(file, peek().line, peek().column, peek().filePos,
                          MALFORMEDEXPR);
  return token(NOToken, 0, 0, 0);
}

void parser::synchronize() {
  advance();
  while (!isEOF()) {
    if (previous().id == NEWLineToken || previous().id == ENDStatementToken)
      return;
    switch (peek().id) {
    case IFToken:
    case WHILEToken:
    case RETURNToken:
    case STRUCTToken:
    case ENUMToken:
      return;
    }
    advance();
  }
}
expression *parser::parse() {
  while (match(NEWLineToken, ENDStatementToken))
    ;
  if (isEOF())
    return nullptr;
  expression *exprs = expr();
  if (errorFound) {
    if (exprs != nullptr) {
      exprs->dealloc();
      // delete exprs;
    }
    errorFound = false;
    synchronize();
    return nullptr;
  }
  return exprs;
}

expression *parser::expr() { return equality(); }

expression *parser::equality() {
  expression *expr = comparison();

  while (match(NOTEQUALToken, EQUALToken)) {
    token operatr = previous();
    expression *right = comparison();
    expr = new binaryExpr(operatr, expr, right);
  }

  return expr;
}

expression *parser::comparison() {
  expression *expr = term();

  while (match(GREATToken, GREATEQUALToken, LESSEQUALToken, LESSToken)) {
    token operatr = previous();
    expression *right = term();
    expr = new binaryExpr(operatr, expr, right);
  }

  return expr;
}

expression *parser::term() {
  expression *expr = factor();

  while (match(ADDToken, SUBToken)) {
    token operatr = previous();
    expression *right = factor();
    expr = new binaryExpr(operatr, expr, right);
  }

  return expr;
}
expression *parser::factor() {
  expression *expr = unary();

  while (match(MULTIPLYToken, DIVIDEToken)) {
    token operatr = previous();
    expression *right = unary();
    expr = new binaryExpr(operatr, expr, right);
  }

  return expr;
}
expression *parser::unary() {

  if (match(SUBToken, NOTToken)) {
    token operatr = previous();
    expression *right = unary();
    return new unaryExpr(operatr, right);
  }

  return primary();
}

expression *parser::primary() {
  if (match(FALSEToken))
    return new literalExpr(FALSEToken);
  if (match(TRUEToken))
    return new literalExpr(TRUEToken);
  if (match(NULLToken))
    return new literalExpr(NULLToken);
  if (match(STRINGLiteralToken, FLOATLiteralToken, NUMBERLiteralToken,
            IDENTIFIERToken)) {

    literalExpr *exprs = new literalExpr(previous().id, previous().literal);
    if (exprs->terminal == ERRORToken) {
      if (exprs != nullptr) {
        exprs->dealloc();
        // delete exprs;
      }
      errorReport.reportError(MALFORMEDEXPR);
      return nullptr;
    }
    return exprs;
  }
  if (match(ENDStatementToken))
    return new literalExpr(ENDStatementToken);
  if (match(LCurlyBracketToken)) {
    expression *exprs = expr();
    if (consume(RCurlyBracketToken).id != NOToken)
      return new groupingExpr(exprs);
    if (exprs != nullptr) {
      exprs->dealloc();
      // delete exprs;
    }
    return nullptr;
  }
  if (match(LBracketToken)) {
    expression *exprs = expr();
    if (consume(RBracketToken).id != NOToken)
      return new groupingExpr(exprs);
    if (exprs != nullptr) {
      exprs->dealloc();
      // delete exprs;
    }
    return nullptr;
  }
  if (match(LRectBracketToken)) {
    expression *exprs = expr();
    if (consume(RRectBracketToken).id != NOToken)
      return new groupingExpr(exprs);
    if (exprs != nullptr) {
      exprs->dealloc();
      // delete exprs;
    }
    return nullptr;
  }
  errorFound = true;
  // std::string str = "Not implemented yet";
  // str += ' ';
  // str += tokenNames[peek().id];
  // errorReport.reportError(str, peek().line, peek().column, MALFORMEDEXPR);
  errorReport.reportError(file, peek().line, peek().column, peek().filePos,
                          MALFORMEDEXPR);
  return nullptr;
}
