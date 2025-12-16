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
  exp(const std::string &str, const tokenId tag);
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
    char *id = nullptr;
    char *str;
    int num;
    struct { // for OP
      astOp op;
      exp *left = nullptr, *right = nullptr;
    } binop;
    struct {
      astOp op;
      exp *expr = nullptr;
    } unaryop;
  };
};

class args {
public:
  // New Args
  args(exp *exp);
  void printArgs();
  void appendArg(args *newArg);
  exp *arg = nullptr;
  args *next = nullptr;
};

class stm {
public:
  stmType tag;
  // New compound statement
  stm(stm *lStm, stm *rStm);
  // New assign statement
  stm(const std::string &id, const tokenId type, exp *exp);
  // New function statement
  stm(const std::string &id, args *args);
  // New if statement
  stm(exp *cond, stm *thenBranch, stm *elseBranch);
  // New while statement
  stm(exp *cond, stm *body);
  void printStm();
  ~stm();
  union {
    struct { // for COMPOUND
      stm *fst = nullptr, *snd = nullptr;
    } compound;
    struct { // for ASSIGN
      char *id = nullptr;
      expType type;
      exp *expr = nullptr;
    } assign;
    struct {
      args *arg = nullptr;
      char *id = nullptr;
    } function;
    struct { // for IF
      exp *cond = nullptr;
      stm *thenBranch = nullptr;
      stm *elseBranch;
    } ifStmt;
    struct { // for WHILE
      exp *cond = nullptr;
      stm *body = nullptr;
    } whileStmt;
  };
};

class func {
public:
  func(const std::string &id, const tokenId returnValue, stm *args);
  ~func();
  void printFunc();

  std::string id;
  expType returnValueTag;
  int numArgs;
  stm *args = nullptr;
  stm *stmt = nullptr;
};
class decl {
public:
  decl(func *ptr, decl *decl);
  // decl(structDecl* ptr);
  ~decl();
  void printDecl();
  enum { DECLFUNCTION, DECLSTRUCT } tag;
  union {
    func *fn = nullptr;
    // structDecl* strt;
  } declaration;
  decl *next = nullptr;
};

void printOp(astOp op);
