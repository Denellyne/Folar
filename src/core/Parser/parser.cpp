#include "parser.h"
#include "Rules/Expressions/literalExpr.h"
#include "Rules/expressions.h"

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
  case LCurlyBracketToken:
    str = tokenNames[peek().id];
    str += " should had been" + tokenNames[LCurlyBracketToken] + '\n';
    break;
  }

  errorFound = true;
  errorReport.reportError(str, peek().line, peek().column, MALFORMEDEXPR);
  return token(ERRORToken, 0, 0);
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
            IDENTIFIERToken))
    return new literalExpr(previous().id, previous().literal);
  if (match(ENDStatementToken))
    return new literalExpr(ENDStatementToken);
  if (match(LCurlyBracketToken)) {
    expression *exprs = expr();
    consume(RCurlyBracketToken);
    return new groupingExpr(exprs);
  }
  if (match(LBracketToken)) {
    expression *exprs = expr();
    consume(RBracketToken);
    return new groupingExpr(exprs);
  }
  if (match(LRectBracketToken)) {
    expression *exprs = expr();
    consume(RRectBracketToken);
    return new groupingExpr(exprs);
  }
  errorFound = true;
  std::string str = "Not implemented yet";
  str += ' ' + tokenNames[peek().id];
  errorReport.reportError(str, peek().line, peek().column, MALFORMEDEXPR);
  return nullptr;
}
