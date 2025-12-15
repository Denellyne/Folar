#pragma once
#include <string>
#include <string_view>
typedef enum {
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
} astOp;
typedef enum {
  ASTID,
  ASTNUM,
  ASTBOOL,
  ASTSTRLITERAL,
  ASTFLOAT,
  ASTDOUBLE,
  ASTUNARYOP,
  ASTBINOP,
} expType;
typedef enum {
  ASTCOMPOUND,
  ASTASSIGN,
  ASTFUNCTION,
  ASTIF,
  ASTWHILE,
} stmType;
struct exp {
  exp();
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
      exp *exp;
    } unaryop;
  };
};

struct args {
  args();
  exp *arg;
  args *next;
};

struct stm {
  stm() {}
  stmType tag;
  union {
    struct { // for COMPOUND
      stm *fst, *snd;
    } compound;
    struct { // for ASSIGN
      std::string id;
      int type;
      exp *expr;
    } assign;
    struct {
      args *args;
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

struct func {
  func();
  expType returnValue;

  std::string id;
  int returnValueTag;
  int numArgs;
  stm *args;
  stm *stm;
};

exp *mkStringLiteral(const std::string_view stringLiteral);
exp *mkId(const std::string_view id);
exp *mkBool(const int b);
exp *mkNum(const int v);
exp *mkFloat(const double v);
exp *mkBinOp(exp *lExp, const astOp op, exp *rExp);
exp *mkUnaryOp(exp *exp, const astOp op);
stm *mkCompound(stm *lStm, stm *rStm);
stm *mkAssign(const std::string_view id, const int type, exp *exp);
stm *mkIncr(const std::string_view id);
stm *mkArgList(args *);
stm *mkFuncCall(const std::string_view id, args *args);
stm *mkIf(exp *cond, stm *thenBranch, stm *elseifBranch, stm *elseBranch);
stm *mkWhile(exp *cond, stm *body);

args *mkArg(exp *expr);
args *appendArgs(args *root, args *newArg);

func *mkFunc(const std::string_view id, const int returnValue, stm *args);

void printStm(stm *);
void printExp(exp *);
void printOp(astOp op);
void printFunc(func *);
void printArgs(args *);
