#include "parser.h"
#include "../ErrorHandler/errorHandler.h"
bool parser::createFilestream(std::string_view str) {
  closeFile();
  file.clear();
  file.open(str.data(), std::fstream::in | std::fstream::binary);
  if (file.fail()) {
    closeFile();
    errorHandler::getInstance().reportError(FILEERROR, str);
    return false;
  }
  return true;
}
void parser::closeFile() {
  // file.clear();
  // file.seekg(0, std::ios::beg);
  if (file.is_open())
    file.close();
  // while (file.is_open())
  //   ;
}
void parser::reportError(unsigned errorType) {
  errorFound = true;
  errorHandler::getInstance().reportError(file, peek().line, peek().column,
                                          peek().filePos, errorType);
}
void parser::reportError(std::string_view customError) {

  errorFound = true;
  errorHandler::getInstance().reportError(file, peek().line, peek().column,
                                          peek().filePos, CUSTOMERROR,
                                          customError);
}
void parser::receiveTokens(const std::vector<token> &token) {
  tokens = std::move(token);
  pos = 0;
  errorFound = 0;
}

constexpr bool parser::matchToType() {
  for (const auto &token : tokenTypes) {
    if (check(token)) {
      advance();
      return true;
    }
  }
  return false;
}

constexpr bool parser::match(const tokenId type) {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
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
    default:
      break;
    }
    advance();
  }
}
decl *parser::parse() {
  while (match(NEWLineToken))
    ;
  if (isEOF())
    return nullptr;

  decl *ptr = declaration();
  if (errorFound) {
    if (ptr)
      delete ptr;

    synchronize();
    return nullptr;
  }
  return ptr;
}

decl *parser::declaration() {

  decl *ptr = nullptr;
  if (matchToType()) {
    ptr = new decl(functionDeclaration(), declaration());
    if (!ptr)
      reportError("Unable to generate AST");
    else
      return ptr;
  }

  return nullptr;
}
func *parser::functionDeclaration() {
  token tag = previous();
  token id = consume(IDENTIFIERToken);
  if (id.id == NOToken) {
    reportError("Function has no name");
    return nullptr;
  }
  if (!match(LCurlyBracketToken)) {
    reportError("No Left arg bracket");
    return nullptr;
  }
  if (!match(RCurlyBracketToken)) {
    reportError("No Right arg bracket");
    return nullptr;
  }
  if (!match(LBracketToken)) {
    reportError("No Left definition bracket");
    return nullptr;
  }

  func *fn = new func(id.literal, tag.id, statements());
  if (!fn) {
    reportError("Function pointer returned null");
    return nullptr;
  }
  if (!match(RBracketToken)) {
    delete fn;
    reportError("No Right definiton bracket");
    return nullptr;
  }
  return fn;
}

stm *parser::statements() {
  stm *stmt = nullptr;
  if (match(LETToken, IDENTIFIERToken)) {
    if (previous().id == LETToken)
      advance();
    stmt = new stm(assignStatement(), statements());
    if (!stmt) {
      reportError("Unable to generate assign statement");
      return nullptr;
    }
    if (consume(ENDStatementToken).id == NOToken) {
      delete stmt;
      reportError("End of statement not found");
      return nullptr;
    }
    // } else if (match(IFToken)) {
    //   stmt = new stm(ifStatement(), statements());
    //   if (!stmt) {
    //     reportError("Unable to generate assign statement");
    //     return nullptr;
    //   }
    //
    // } else if (match(WHILEToken)) {
    //   stmt = new stm(whileStatement(), statements());
    //   if (!stmt) {
    //     reportError("Unable to generate assign statement");
    //     return nullptr;
    //   }
    return stmt;
  }
  return nullptr;
}

stm *parser::assignStatement() {
  token id = previous();
  if (id.id != IDENTIFIERToken) {
    reportError("No name for variable given");
    return nullptr;
  }
  if (consume(TYPEIdentifierToken).id == NOToken) {
    reportError("No type identification found for variable");
    return nullptr;
  }
  if (!matchToType()) {
    reportError("No type identification found for variable");
    return nullptr;
  }
  token type = previous();

  if (match(ASSIGNToken)) {
    exp *value = expr();
    printf("%s %s\n", id.literal.c_str(), type.literal.c_str());
    stm *stmt = new stm(id.literal, type.id, value);

    printf("%s %s\n", id.literal.c_str(), type.literal.c_str());
    if (!stmt) {
      reportError("Unable to assign to expression");
      return nullptr;
    }

    if (value) {
      delete value;
      value = nullptr;
    }

    return stmt;
  }
  if (consume(ENDStatementToken).id == NOToken) {
    reportError("End of statement not found");
    return nullptr;
  }

  return new stm(id.literal, type.id, nullptr);
}
exp *parser::expr() { return bitwise(); }

exp *parser::bitwise() {
  exp *expr = orExp();
  if (!expr) {
    reportError("Or expression returned null");
    return nullptr;
  }
  while (match(BITWISEANDToken, BITWISEORToken, BITWISEXORToken)) {
    token operatr = previous();
    exp *right = orExp();
    astOp op;
    switch (operatr.id) {
    case BITWISEANDToken:
      op = ASTBAND;
      break;
    case BITWISEORToken:
      op = ASTBOR;
      break;
    case BITWISEXORToken:
      op = ASTXOR;
      break;
    default:
      op = ASTERROR;
      break;
    }
    if (op == ASTERROR || !right) {
      reportError("No matching operator in bitwise expression");
      delete expr;
      if (right)
        delete right;
      return nullptr;
    }
    expr = new exp(expr, op, right);
  }
  if (!expr) {
    reportError("Final bitwise expression is null");
    return nullptr;
  }
  return expr;
}

exp *parser::orExp() {
  exp *expr = andExp();
  if (!expr) {
    reportError("And expression returned null");
    return nullptr;
  }
  while (match(ORToken)) {
    token operatr = previous();
    exp *right = andExp();
    astOp op = ASTOR;
    if (!right) {
      reportError("No matching operator in and expression");
      delete expr;
      return nullptr;
    }
    expr = new exp(expr, op, right);
  }
  if (!expr) {
    reportError("Final or expression is null");
    return nullptr;
  }
  return expr;
}
exp *parser::andExp() {
  exp *expr = compExp();
  if (!expr) {
    reportError("Comparison expression returned null");
    return nullptr;
  }
  while (match(ANDToken)) {
    token operatr = previous();
    exp *right = compExp();
    astOp op = ASTAND;
    if (!right) {
      reportError("No matching operator in comparison expression");
      delete expr;
      if (right)
        delete right;
      return nullptr;
    }
    expr = new exp(expr, op, right);
  }
  if (!expr) {
    reportError("Final and expression is null");
    return nullptr;
  }
  return expr;
}

exp *parser::compExp() {
  exp *expr = addExp();
  if (!expr) {
    reportError("Add expression returned null");
    return nullptr;
  }
  if (match(EQUALToken, NOTEQUALToken, GREATEQUALToken, GREATToken, LESSToken,
            LESSEQUALToken)) {
    token operatr = previous();
    exp *right = addExp();
    astOp op = convertOp(operatr);
    if (!right) {
      reportError("No matching operator in add expression");
      delete expr;
      if (right)
        delete right;
      return nullptr;
    }
    expr = new exp(expr, op, right);
  }
  if (!expr) {
    reportError("Final comparison expression is null");
    return nullptr;
  }
  return expr;
}
exp *parser::addExp() {
  exp *expr = multExp();
  if (!expr) {
    reportError("Mult expression returned null");
    return nullptr;
  }
  if (match(ADDToken, SUBToken)) {
    token operatr = previous();
    exp *right = multExp();
    astOp op = convertOp(operatr);
    if (!right) {
      reportError("No matching operator in mult expression");
      delete expr;
      if (right)
        delete right;
      return nullptr;
    }
    expr = new exp(expr, op, right);
  }
  if (!expr) {
    reportError("Final add expression is null");
    return nullptr;
  }
  return expr;
}

exp *parser::multExp() {
  exp *expr = powExp();
  if (!expr) {
    reportError("Power expression returned null");
    return nullptr;
  }
  if (match(MODULUSToken, MULTIPLYToken, DIVIDEToken)) {
    token operatr = previous();
    exp *right = powExp();
    astOp op = convertOp(operatr);
    if (!right) {
      reportError("No matching operator in power expression");
      delete expr;
      if (right)
        delete right;
      return nullptr;
    }
    expr = new exp(expr, op, right);
  }
  if (!expr) {
    reportError("Final mult expression is null");
    return nullptr;
  }
  return expr;
}

exp *parser::powExp() {
  exp *expr = unaryExp();
  if (!expr) {
    reportError("Unary expression returned null");
    return nullptr;
  }
  if (match(POWToken)) {
    token operatr = previous();
    exp *right = unaryExp();
    astOp op = convertOp(operatr);
    if (!right) {
      reportError("No matching operator in unary expression");
      delete expr;
      if (right)
        delete right;
      return nullptr;
    }
    expr = new exp(expr, op, right);
  }
  if (!expr) {
    reportError("Final power expression is null");
    return nullptr;
  }
  return expr;
}

// exp *parser::equality() {
//   exp *expr = comparison();
//
//   while (match(NOTEQUALToken, EQUALToken)) {
//     token operatr = previous();
//     exp *right = comparison();
//     expr = new binaryExpr(operatr, expr, right);
//   }
//
//   return expr;
// }
//
// exp *parser::comparison() {
//   exp *expr = term();
//
//   while (match(GREATToken, GREATEQUALToken, LESSEQUALToken, LESSToken)) {
//     token operatr = previous();
//     exp *right = term();
//     expr = new binaryExpr(operatr, expr, right);
//   }
//
//   return expr;
// }
//
// exp *parser::term() {
//   exp *expr = factor();
//
//   while (match(ADDToken, SUBToken)) {
//     token operatr = previous();
//     exp *right = factor();
//     expr = new binaryExpr(operatr, expr, right);
//   }
//
//   return expr;
// }
// exp *parser::factor() {
//   exp *expr = unary();
//
//   while (match(MULTIPLYToken, DIVIDEToken)) {
//     token operatr = previous();
//     exp *right = unary();
//     expr = new binaryExpr(operatr, expr, right);
//   }
//
//   return expr;
// }
// exp *parser::unary() {
//
//   if (match(SUBToken, NOTToken)) {
//     token operatr = previous();
//     exp *right = unary();
//     return new unaryExpr(operatr, right);
//   }
//
//   return primary();
// }
//
// exp *parser::primary() {
//   if (match(FALSEToken))
//     return new literalExpr(FALSEToken);
//   if (match(TRUEToken))
//     return new literalExpr(TRUEToken);
//   if (match(NULLToken))
//     return new literalExpr(NULLToken);
//   if (match(STRINGLiteralToken, FLOATLiteralToken, NUMBERLiteralToken,
//             IDENTIFIERToken, CHARLiteralToken)) {
//     literalExpr *exprs = new literalExpr(previous().id, previous().literal);
//
//     if (exprs->terminal == ERRORToken) {
//       if (exprs != nullptr)
//         exprs->dealloc();
//
//       reportError("Couldn't find terminal token");
//       return nullptr;
//     }
//     return exprs;
//   }
//   // if (match(ENDStatementToken))
//   // return new literalExpr(ENDStatementToken);
//   if (match(LCurlyBracketToken)) {
//     exp *exprs = declaration();
//     if (consume(RCurlyBracketToken).id == NOToken) {
//       if (exprs != nullptr)
//         exprs->dealloc();
//
//       reportError("Expected )");
//       return nullptr;
//     }
//     return new groupingExpr(exprs);
//   }
//   if (match(LBracketToken)) {
//
//     exp *exprs = declaration();
//     if (consume(RBracketToken).id == NOToken) {
//       if (exprs != nullptr)
//         exprs->dealloc();
//
//       reportError("Expected }");
//       return nullptr;
//     }
//     return new groupingExpr(exprs);
//   }
//   if (match(LRectBracketToken)) {
//     exp *exprs = declaration();
//     if (consume(RRectBracketToken).id == NOToken) {
//       if (exprs != nullptr)
//         exprs->dealloc();
//
//       reportError("Expected ]");
//       return nullptr;
//     }
//     return new groupingExpr(exprs);
//   }
//   reportError(MALFORMEDEXPR);
//   return nullptr;
// }
//
// exp *parser::variableDeclaration() {
//
//   if (match(MUTABLEToken)) {
//     token identifier = advance();
//     if (identifier.id != IDENTIFIERToken) {
//       reportError("Expected variable name");
//       return nullptr;
//     }
//     if (consume(TYPEIdentifierToken).id == NOToken) {
//       reportError("Expected : after variable name");
//       return nullptr;
//     }
//
//     token tk = advance();
//     if (tk.id < INT8Token || tk.id > FLOAT128Token) {
//       const std::string str =
//           "Expected type for variable,got instead " + tokenNames[tk.id];
//       reportError(str);
//       return nullptr;
//     }
//     exp *exprs = nullptr;
//     if (match(ASSIGNToken)) {
//       exprs = equality();
//       if (exprs == nullptr) {
//         reportError("Expected expression after =");
//         return nullptr;
//       }
//     }
//     if (consume(ENDStatementToken).id == NOToken) {
//       if (exprs)
//         exprs->dealloc();
//       reportError("End of statement not found");
//       return nullptr;
//     }
//
//     return new variableExpr(tk, exprs, identifier.literal, true);
//   }
//
//   token identifier = advance();
//   if (identifier.id != IDENTIFIERToken) {
//     reportError("Expected variable name");
//     return nullptr;
//   }
//   if (consume(TYPEIdentifierToken).id == NOToken) {
//     reportError("Expected : after variable name");
//     return nullptr;
//   }
//
//   token tk = advance();
//   if (tk.id < INT8Token || tk.id > FLOAT128Token) {
//     const std::string str =
//         "Expected type for variable,got instead " + tokenNames[tk.id];
//     reportError(str);
//     return nullptr;
//   }
//   if (consume(ASSIGNToken).id == NOToken) {
//     reportError(
//         "Can't initialize const variable without assigning to expression");
//     return nullptr;
//   }
//   exp *exprs = equality();
//   if (exprs == nullptr) {
//     reportError("Expected expression after =");
//     return nullptr;
//   }
//   if (consume(ENDStatementToken).id == NOToken) {
//     exprs->dealloc();
//     reportError("End of statement not found");
//     return nullptr;
//   }
//   return new variableExpr(tk, exprs, identifier.literal);
// }
