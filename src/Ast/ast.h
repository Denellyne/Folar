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
class exp {
public:
  exp(const std::string_view str, const expType tag);
  exp(const double v);
  exp(const int v, const expType tag);
  exp(exp *lExp, const astOp op, exp *rExp);
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
      exp *exp;
    } unaryop;
  };
};

class args {
public:
  args(exp *exp);
  void printArgs();
  void appendArg(args *newArg);
  exp *arg;
  args *next;
};

class stm {
public:
  stmType tag;
  stm(stm *lStm, stm *rStm);
  stm(const std::string_view id, const int type, exp *exp);
  stm(const std::string_view id, args *args);
  stm(exp *cond, stm *thenBranch, stm *elseBranch);
  stm(exp *cond, stm *body);
  void printStm();
  ~stm();
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

class func {
public:
  func(const std::string_view id, const int returnValue, stm *args);
  void printFunc();
  expType returnValue;

  std::string id;
  int returnValueTag;
  int numArgs;
  stm *args;
  stm *stm;
};

void printOp(astOp op);
