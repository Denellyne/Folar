#pragma once
#include "../Tokens/tokens.h"
#include <string>
#include <variant>
enum astOp {
  ASTERROR,
  ASTPOW,
  ASTPLUS,
  ASTMINUS,
  ASTTIMES,
  ASTDIV,
  ASTBAND,
  ASTBOR,
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
private:
  struct binOp { // for OP
    binOp(exp *lExp, exp *rExp, astOp op) : left(lExp), right(rExp), op(op) {}
    ~binOp() {
      delete this->left;
      delete this->right;
    }
    astOp op = ASTERROR;
    exp *left = nullptr, *right = nullptr;
  };
  struct unaryOp {

    unaryOp(exp *expr, astOp op) : expr(expr), op(op) {}
    ~unaryOp() { delete this->expr; }
    astOp op = ASTERROR;
    exp *expr = nullptr;
  };

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
  std::variant<binOp, unaryOp, double, int, std::string> variant = 0;
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
private:
  struct compound { // for COMPOUND
    compound(stm *fst, stm *snd) : fst(fst), snd(snd) {}
    ~compound() {
      if (this->fst)
        delete this->fst;
      if (this->snd)
        delete this->snd;
    }
    stm *fst = nullptr, *snd = nullptr;
  };
  struct assign { // for ASSIGN

    assign(const std::string &id, exp *expr, expType type)
        : id(id), expr(expr), type(type) {}

    ~assign() {
      if (this->expr)
        delete this->expr;
    }
    std::string id;
    expType type;
    exp *expr = nullptr;
  };
  struct function {
    function(const std::string &id, args *arg) : id(id), arg(arg) {}
    ~function() {
      if (this->arg)
        delete this->arg;
    }
    args *arg = nullptr;
    std::string id;
  };
  struct ifStmt { // for IF
    ifStmt(exp *cond, stm *thenBranch, stm *elseBranch)
        : cond(cond), thenBranch(thenBranch), elseBranch(elseBranch) {}
    ~ifStmt() {
      delete this->cond;
      if (this->thenBranch)
        delete this->thenBranch;
      if (this->elseBranch)
        delete this->elseBranch;
    }
    exp *cond = nullptr;
    stm *thenBranch = nullptr;
    stm *elseBranch = nullptr;
  };
  struct whileStmt { // for WHILE
    whileStmt(exp *cond, stm *body) : cond(cond), body(body) {}
    ~whileStmt() {
      delete this->cond;
      if (this->body)
        delete this->body;
    }
    exp *cond = nullptr;
    stm *body = nullptr;
  };

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
  std::variant<compound, assign, function, ifStmt, whileStmt> variant =
      compound(nullptr, nullptr);
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
  std::variant<func *> variant;
  decl *next = nullptr;
};

void printOp(astOp op);
