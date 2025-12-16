#include "ast.h"
#include "../ErrorHandler/errorHandler.h"
#include <string_view>
astToken convertToken(tokenId tag) {
  astToken a;
  switch (tag) {
  case INT8Token:
  retuINT8Token:
    a.exp = ASTNUM;
    break;
  }
  return a;
}

exp::exp(const std::string_view str, const tokenId tag)
    : tag(convertToken(tag).exp) {
  if (tag == ASTSTRLITERAL)
    this->str = str;
  else if (tag == ASTID)
    this->id = str;
  else
    errorHandler::getInstance().reportError(INVALIDAST, str);
}

exp::exp(const double v) : val(v) { this->tag = ASTFLOAT; }
exp::exp(const int v, const tokenId tag) : num(v), tag(convertToken(tag).exp) {}
exp::exp(exp *lExp, const astOp op, exp *rExp) {
  this->tag = ASTBINOP;
  this->binop.left = std::move(lExp);
  this->binop.right = std::move(rExp);
  this->binop.op = op;
}

exp::exp(exp *expr, const astOp op) {
  this->tag = ASTUNARYOP;
  this->unaryop.op = op;
  this->unaryop.expr = std::move(expr);
}
exp::~exp() {
  if (this->tag == ASTBINOP) {
    delete this->binop.left;
    delete this->binop.right;
  } else if (this->tag == ASTUNARYOP)
    delete this->unaryop.expr;
}

stm::stm(stm *lStm, stm *rStm) {

  this->tag = ASTCOMPOUND;
  this->compound.fst = std::move(lStm);
  this->compound.snd = std::move(rStm);
}

stm::stm(const std::string_view id, const tokenId type, exp *exp) {

  this->tag = ASTASSIGN;
  this->assign.id = id;
  this->assign.expr = std::move(exp);
  expType convertedType = convertToken(type).exp;

  if (convertedType == ASTNOTYPE && exp)
    this->assign.type = exp->tag;
  else
    this->assign.type = convertedType;
}

stm::stm(const std::string_view id, args *args) {

  this->tag = ASTFUNCTION;
  this->function.arg = std::move(args);
  this->function.id = id;
}

stm::stm(exp *cond, stm *thenBranch, stm *elseBranch) {
  this->tag = ASTIF;
  this->ifStmt.cond = cond;
  this->ifStmt.thenBranch = std::move(thenBranch);
  this->ifStmt.elseBranch = std::move(elseBranch);
}
stm::stm(exp *cond, stm *body) {
  this->tag = ASTWHILE;
  this->whileStmt.cond = std::move(cond);
  this->whileStmt.body = std::move(body);
}

stm::~stm() {
  switch (this->tag) {
  case ASTCOMPOUND:
    if (this->compound.fst)
      delete this->compound.fst;
    if (this->compound.snd)
      delete this->compound.snd;
    break;
  case ASTASSIGN:
    if (this->assign.expr)
      delete this->assign.expr;
    break;
  case ASTFUNCTION:
    if (this->function.arg)
      delete this->function.arg;
    break;
  case ASTIF:
    if (this->ifStmt.cond)
      delete this->ifStmt.cond;
    if (this->ifStmt.thenBranch)
      delete this->ifStmt.thenBranch;
    if (this->ifStmt.elseBranch)
      delete this->ifStmt.elseBranch;
    break;
  case ASTWHILE:
    if (this->whileStmt.cond)
      delete this->whileStmt.cond;
    if (this->whileStmt.body)
      delete this->whileStmt.body;
    break;
  }
}
args::args(exp *expr) : arg(expr) { this->next = nullptr; }
void args::appendArg(args *newArg) {
  args *head = this;
  while (head->next)
    head = head->next;

  head->next = std::move(newArg);
}

func::func(const std::string_view id, const tokenId returnValue, stm *args)
    : id(id), returnValueTag(convertToken(returnValue).exp), args(args) {}

func::~func() {
  if (this->args)
    delete this->args;
  if (this->stmt)
    delete this->stmt;
}

void func::printFunc() {
  if (!this)
    return;
  printf("%s %u ", this->id.c_str(), this->returnValueTag);
  printf("( ");
  this->args->printStm();
  printf(") ");
  printf("( ");
  this->stmt->printStm();
  printf(") ");
}

decl::decl(func *fn, decl *ptr) : next(std::move(ptr)) {
  this->tag = DECLFUNCTION;
  this->declaration.fn = std::move(fn);
}
decl::~decl() {
  switch (this->tag) {

  case DECLFUNCTION:
    if (this->declaration.fn)
      delete this->declaration.fn;
    break;
  case DECLSTRUCT:
    // delete this->declaration.strt;
    break;
  }
}
void decl::printDecl() {
  if (!this)
    return;
  switch (this->tag) {
  case DECLFUNCTION:
    this->declaration.fn->printFunc();
    break;
  case DECLSTRUCT:
    //  this->declaration.strt->printStruct();
    break;
  }
  return this->next->printDecl();
}

void stm::printStm() {
  if (!this)
    return;

  switch (this->tag) {
  case ASTASSIGN:
    printf("(");
    printf("%s", this->assign.id.c_str());
    switch (this->assign.type) {
    case ASTFLOAT:
      printf(" ASTFLOAT");
      break;
    case ASTID:
      printf(" ASTID");
      break;
    case ASTNUM:
      printf(" INTEGER");
      break;
    case ASTSTRLITERAL:
      printf(" STRING");
      break;
    case ASTBOOL:
      printf(" ASTBOOL");
      break;
    default:
      break;
    }
    if (this->assign.expr) {
      printf(" (");
      this->assign.expr->printExp();
      printf(")");
    }
    printf(")");

    break;
  case ASTCOMPOUND:
    this->compound.fst->printStm();
    printf(" ");
    this->compound.snd->printStm();
    break;
  case ASTFUNCTION: {
    printf("%s", this->function.id.c_str());
    printf("(");
    this->function.arg->printArgs();
    printf(")");
  } break;
  case ASTIF: {

    int hasElse = 0;
    printf("IF");
    printf(" THEN ");

    if (this->ifStmt.elseBranch) {
      printf(" ELSE ");
      hasElse = 1;
    }

    this->ifStmt.cond->printExp();
    printf(" (");
    this->ifStmt.thenBranch->printStm();
    printf(")");

    if (hasElse) {
      printf(" (");
      this->ifStmt.elseBranch->printStm();
      printf(")");
    }
  } break;
  case ASTWHILE:
    printf("WHILE");
    printf(" DO ");
    this->whileStmt.cond->printExp();
    printf(" ");
    this->whileStmt.body->printStm();
    break;
  }
}

void args::printArgs() {
  if (!this)
    return;
  this->arg->printExp();
  if (this->next)
    printf(" ");
  return this->next->printArgs();
}
void exp::printExp() {
  if (!this)
    return;

  switch (this->tag) {
  case ASTDOUBLE:
  case ASTFLOAT:
    printf("%f", this->val);
    break;
  case ASTID:
    printf("%s", this->id.c_str());
    break;
  case ASTBINOP:
    printf("(");
    printOp(this->binop.op);
    this->binop.left->printExp();
    printf(" ");
    this->binop.right->printExp();
    printf(")");
    break;
  case ASTUNARYOP:
    printf("(");
    printOp(this->unaryop.op);
    this->unaryop.expr->printExp();
    printf(")");
    break;
  case ASTSTRLITERAL:
    printf("%s", this->str.c_str());
    break;
  case ASTNUM:
    printf("%d", this->num);
    break;
  case ASTBOOL:
    if (this->num)
      printf("TRUE");
    else
      printf("FALSE");
    break;
  }
}

void printOp(const astOp op) {
  switch (op) {
  case ASTPOW:
    printf("POW ");
    break;
  case ASTPLUS:
    printf("PLUS ");
    break;
  case ASTMINUS:
    printf("MINUS ");
    break;
  case ASTTIMES:
    printf("MULT ");
    break;
  case ASTDIV:
    printf("DIV ");
    break;
  case ASTAND:
    printf("AND ");
    break;
  case ASTOR:
    printf("OR ");
    break;
  case ASTNOT:
    printf("NOT ");
    break;
  case ASTEQ:
    printf("EQUAL ");
    break;
  case ASTNEQ:
    printf("NOT EQUAL ");
    break;
  case ASTLT:
    printf("LESSER ");
    break;
  case ASTGT:
    printf("GREATER ");
    break;
  case ASTLE:
    printf("LESS EQUAL ");
    break;
  case ASTGE:
    printf("GREAT EQUAL ");
    break;
  case ASTXOR:
    printf("XOR ");
    break;
  }
}
