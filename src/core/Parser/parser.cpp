#include "parser.h"
#include "Rules/Expressions/binaryExpr.h"
#include "Rules/Expressions/groupingExpr.h"
#include "Rules/Expressions/literalExpr.h"
#include "Rules/Expressions/unaryExpr.h"
#include "Rules/Expressions/variableExpr.h"
#include "Rules/expressions.h"
#include <cmath>
bool parser::createFilestream(std::string_view str) {
  closeFile();
  file.clear();
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
void parser::reportError(unsigned errorType) {
  errorFound = true;
  errorReport.reportError(file, peek().line, peek().column, peek().filePos,
                          errorType);
}
void parser::reportError(std::string_view customError) {

  errorFound = true;
  errorReport.reportError(file, peek().line, peek().column, peek().filePos,
                          CUSTOMERROR, customError);
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

  return token(NOToken, 0, 0, 0);
}

void parser::synchronize() {
  errorFound = false;
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
    case LETToken:
      return;
    }
    advance();
  }
}
expression *parser::parse() {
  while (match(NEWLineToken))
    ;
  if (isEOF())
    return nullptr;

  expression *exprs = declaration();
  if (errorFound) {
    if (exprs != nullptr)
      exprs->dealloc();

    synchronize();
    return nullptr;
  }
  return exprs;
}

expression *parser::declaration() {

  if (match(LETToken))
    return variableDeclaration();

  return expr();
}

expression *parser::expr() { return assignment(); }

expression *parser::assignment() {
  expression *expr = equality();

  if (match(ASSIGNToken)) {
    expression *value = assignment();
    if (dynamic_cast<literalExpr *>(expr) != nullptr) {

      variableExpr *var =
          new variableExpr(token(NOToken, 0, 0, 0), value,
                           dynamic_cast<literalExpr *>(expr)->literal.str);

      if (expr)
        expr->dealloc();
      expr = nullptr;

      if (consume(ENDStatementToken).id == NOToken) {
        if (var)
          var->dealloc();
        var = nullptr;
        reportError("End of statement not found");
        return nullptr;
      }
      return var;
    }
    if (expr)
      expr->dealloc();

    if (value)
      value->dealloc();

    value = nullptr;
    expr = nullptr;
    reportError("Invalid assignment");
    return nullptr;
  }

  return expr;
}

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
            IDENTIFIERToken, CHARLiteralToken)) {
    literalExpr *exprs = new literalExpr(previous().id, previous().literal);

    if (exprs->terminal == ERRORToken) {
      if (exprs != nullptr)
        exprs->dealloc();

      reportError("Couldn't find terminal token");
      return nullptr;
    }
    return exprs;
  }
  // if (match(ENDStatementToken))
  // return new literalExpr(ENDStatementToken);
  if (match(LCurlyBracketToken)) {
    expression *exprs = declaration();
    if (consume(RCurlyBracketToken).id == NOToken) {
      if (exprs != nullptr)
        exprs->dealloc();

      reportError("Expected )");
      return nullptr;
    }
    return new groupingExpr(exprs);
  }
  if (match(LBracketToken)) {

    expression *exprs = declaration();
    if (consume(RBracketToken).id == NOToken) {
      if (exprs != nullptr)
        exprs->dealloc();

      reportError("Expected }");
      return nullptr;
    }
    return new groupingExpr(exprs);
  }
  if (match(LRectBracketToken)) {
    expression *exprs = declaration();
    if (consume(RRectBracketToken).id == NOToken) {
      if (exprs != nullptr)
        exprs->dealloc();

      reportError("Expected ]");
      return nullptr;
    }
    return new groupingExpr(exprs);
  }
  reportError(MALFORMEDEXPR);
  return nullptr;
}

expression *parser::variableDeclaration() {

  if (match(MUTABLEToken)) {
    token identifier = advance();
    if (identifier.id != IDENTIFIERToken) {
      reportError("Expected variable name");
      return nullptr;
    }
    if (consume(TYPEIdentifierToken).id == NOToken) {
      reportError("Expected : after variable name");
      return nullptr;
    }

    token tk = advance();
    if (tk.id < INT8Token || tk.id > FLOAT128Token) {
      const std::string str =
          "Expected type for variable,got instead " + tokenNames[tk.id];
      reportError(str);
      return nullptr;
    }
    expression *exprs = nullptr;
    if (match(ASSIGNToken)) {
      exprs = equality();
      if (exprs == nullptr) {
        reportError("Expected expression after =");
        return nullptr;
      }
    }
    if (consume(ENDStatementToken).id == NOToken) {
      if (exprs)
        exprs->dealloc();
      reportError("End of statement not found");
      return nullptr;
    }

    return new variableExpr(tk, exprs, identifier.literal, true);
  }

  token identifier = advance();
  if (identifier.id != IDENTIFIERToken) {
    reportError("Expected variable name");
    return nullptr;
  }
  if (consume(TYPEIdentifierToken).id == NOToken) {
    reportError("Expected : after variable name");
    return nullptr;
  }

  token tk = advance();
  if (tk.id < INT8Token || tk.id > FLOAT128Token) {
    const std::string str =
        "Expected type for variable,got instead " + tokenNames[tk.id];
    reportError(str);
    return nullptr;
  }
  if (consume(ASSIGNToken).id == NOToken) {
    reportError(
        "Can't initialize const variable without assigning to expression");
    return nullptr;
  }
  expression *exprs = equality();
  if (exprs == nullptr) {
    reportError("Expected expression after =");
    return nullptr;
  }
  if (consume(ENDStatementToken).id == NOToken) {
    exprs->dealloc();
    reportError("End of statement not found");
    return nullptr;
  }
  return new variableExpr(tk, exprs, identifier.literal);
}
