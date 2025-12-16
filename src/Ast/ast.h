#pragma once
#include "../Tokens/tokens.h"
#include <string>
#include <string_view>
enum astOp {
  ASTPOW,
  ASTPLUS,
  ASTMINUS,
  ASTTIMES,
  ASTDIV,
  ASTAND,
  ASTOR,
  ASTNOT,
  ASTXOR,
  ASTEQ,
  ASTNEQ,
  ASTLT,
  ASTGT,
  ASTLE,
  ASTGE
};
enum expType {
  ASTNOTYPE,
  ASTID,
  ASTNUM,
  ASTBOOL,
  ASTSTRLITERAL,
  ASTFLOAT,
  ASTDOUBLE,
  ASTUNARYOP,
  ASTBINOP,
};
enum stmType {
  ASTCOMPOUND,
  ASTASSIGN,
  ASTFUNCTION,
  ASTIF,
  ASTWHILE,
};
union astToken {
  astOp op;
  expType exp;
  stmType stmt;
};

astToken convertToken(tokenId tag);

class exp {
public:
  // New Id or new String Literal
  exp(const std::string_view str, const tokenId tag);
  // New Float
  exp(const double v);
  // New Int or Bool
  exp(const int v, const tokenId tag);
  // New BINOP
  exp(exp *lExp, const astOp op, exp *rExp);
  // New UNARYOP
  exp(exp *expr, const astOp op);
  void printExp();
  ~exp();
  expType tag;
  union {
    double val;
    std::string id;
    std::string str;
    int num;
    struct { // for OP
      astOp op;
      exp *left, *right;
    } binop;
    struct {
      astOp op;
      exp *expr;
    } unaryop;
  };
};

class args {
public:
  // New Args
  args(exp *exp);
  void printArgs();
  void appendArg(args *newArg);
  exp *arg;
  args *next;
};

class stm {
public:
  stmType tag;
  // New compound statement
  stm(stm *lStm, stm *rStm);
  // New assign statement
  stm(const std::string_view id, const tokenId type, exp *exp);
  // New function statement
  stm(const std::string_view id, args *args);
  // New if statement
  stm(exp *cond, stm *thenBranch, stm *elseBranch);
  // New while statement
  stm(exp *cond, stm *body);
  void printStm();
  ~stm();
  union {
    struct { // for COMPOUND
      stm *fst, *snd;
    } compound;
    struct { // for ASSIGN
      std::string id;
      expType type;
      exp *expr;
    } assign;
    struct {
      args *arg;
      std::string id;
    } function;
    struct { // for IF
      exp *cond;
      stm *thenBranch;
      stm *elseBranch;
    } ifStmt;
    struct { // for WHILE
      exp *cond;
      stm *body;
    } whileStmt;
  };
};

class func {
public:
  func(const std::string_view id, const tokenId returnValue, stm *args);
  ~func();
  void printFunc();

  std::string id;
  expType returnValueTag;
  int numArgs;
  stm *args;
  stm *stmt;
};
class decl {
public:
  decl(func *ptr, decl *decl);
  // decl(structDecl* ptr);
  ~decl();
  void printDecl();
  enum { DECLFUNCTION, DECLSTRUCT } tag;
  union {
    func *fn;
    // structDecl* strt;
  } declaration;
  decl *next;
};

void printOp(astOp op);
