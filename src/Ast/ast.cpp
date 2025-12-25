#include "ast.h"
#include "../ErrorHandler/errorHandler.h"
#include <string>
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

exp::exp(const std::string &str, const tokenId tag)
    : tag(convertToken(tag).exp) {
  if (this->tag == ASTSTRLITERAL || this->tag == ASTID)
    this->variant = str;
  else
    errorHandler::getInstance().reportError(INVALIDAST, str);
}

exp::exp(const double v) {
  this->variant = v;
  this->tag = ASTFLOAT;
}
exp::exp(const int v, const tokenId tag) : tag(convertToken(tag).exp) {
  this->variant = v;
}
exp::exp(exp *lExp, const astOp op, exp *rExp) {
  this->tag = ASTBINOP;
  this->variant = binOp(std::move(lExp), std::move(rExp), op);
}

exp::exp(exp *expr, const astOp op) {
  this->tag = ASTUNARYOP;
  this->variant = unaryOp(std::move(expr), op);
}
exp::~exp() {}

stm::stm(stm *lStm, stm *rStm) {

  this->tag = ASTCOMPOUND;
  this->variant = compound(std::move(lStm), std::move(rStm));
}

stm::stm(const std::string &id, const tokenId type, exp *exp) {

  this->tag = ASTASSIGN;
  expType convertedType = convertToken(type).exp;

  if (convertedType == ASTNOTYPE && exp)
    convertedType = exp->tag;

  this->variant = assign(id, std::move(exp), convertedType);
}

stm::stm(const std::string &id, args *args) {

  this->tag = ASTFUNCTION;
  this->variant = function(id, std::move(args));
}

stm::stm(exp *cond, stm *thenBranch, stm *elseBranch) {
  this->tag = ASTIF;
  this->variant =
      ifStmt(std::move(cond), std::move(thenBranch), std::move(elseBranch));
}
stm::stm(exp *cond, stm *body) {
  this->tag = ASTWHILE;
  this->variant = whileStmt(std::move(cond), std::move(body));
}

stm::~stm() {}
args::args(exp *expr) : arg(expr) { this->next = nullptr; }
void args::appendArg(args *newArg) {
  args *head = this;
  while (head->next)
    head = head->next;

  head->next = std::move(newArg);
}

func::func(const std::string &id, const tokenId returnValue, stm *args)
    : id(id), returnValueTag(convertToken(returnValue).exp), args(args) {}

func::~func() {
  if (this->args)
    delete this->args;
  if (this->stmt)
    delete this->stmt;
}

void func::printFunc() {
  // if (!this)
  //   return;

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
  this->variant = std::move(fn);
}
decl::~decl() {
  switch (this->tag) {

  case DECLFUNCTION: {
    if (func *fn = std::get<func *>(this->variant); fn)
      delete fn;
  } break;
  case DECLSTRUCT:
    // delete this->declaration.strt;
    break;
  }
}
void decl::printDecl() {
  // if (!this)
  //   return;

  switch (this->tag) {
  case DECLFUNCTION:
    if (func *fn = std::get<func *>(this->variant); fn)
      fn->printFunc();
    break;
  case DECLSTRUCT:
    //  this->declaration.strt->printStruct();
    break;
  }
  return this->next->printDecl();
}

void stm::printStm() {
  // if (!this)
  //   return;

  switch (this->tag) {
  case ASTASSIGN: {
    assign &ref = std::get<assign>(this->variant);
    printf("(");
    printf("%s", ref.id.c_str());
    switch (ref.type) {
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
    if (ref.expr) {
      printf(" (");
      ref.expr->printExp();
      printf(")");
    }
    printf(")");
  }

  break;
  case ASTCOMPOUND: {

    compound &ref = std::get<compound>(this->variant);
    if (ref.fst)
      ref.fst->printStm();
    if (ref.snd) {
      printf(" ");
      ref.snd->printStm();
    }
  } break;
  case ASTFUNCTION: {
    function &ref = std::get<function>(this->variant);
    printf("%s", ref.id.c_str());
    printf("(");
    if (ref.arg)
      ref.arg->printArgs();
    printf(")");
  } break;
  case ASTIF: {

    ifStmt &ref = std::get<ifStmt>(this->variant);
    int hasElse = 0;
    printf("IF");
    printf(" THEN ");
    if (ref.elseBranch) {
      printf(" ELSE ");
      hasElse = 1;
    }

    if (ref.cond)
      ref.cond->printExp();

    printf(" (");
    if (ref.thenBranch)
      ref.thenBranch->printStm();
    printf(")");

    if (hasElse) {
      printf(" (");
      ref.elseBranch->printStm();
      printf(")");
    }
  } break;
  case ASTWHILE:
    printf("WHILE");
    printf(" DO ");

    whileStmt &ref = std::get<whileStmt>(this->variant);
    if (ref.cond)
      ref.cond->printExp();
    if (ref.body) {
      printf(" ");
      ref.body->printStm();
    }
    break;
  }
}

void args::printArgs() {
  // if (!this)
  //   return;

  this->arg->printExp();
  if (this->next)
    printf(" ");
  return this->next->printArgs();
}
void exp::printExp() {
  // if (!this)
  //   return;

  switch (this->tag) {
  case ASTDOUBLE:
  case ASTFLOAT:
    printf("%f", std::get<double>(this->variant));
    break;
  case ASTID:
  case ASTSTRLITERAL:
    printf("%s", std::get<std::string>(this->variant).c_str());
    break;
  case ASTBINOP: {
    binOp &ref = std::get<binOp>(this->variant);
    printf("(");
    printOp(ref.op);
    ref.left->printExp();
    printf(" ");
    ref.right->printExp();
    printf(")");
  } break;
  case ASTUNARYOP: {
    unaryOp &ref = std::get<unaryOp>(this->variant);
    printf("(");
    printOp(ref.op);
    ref.expr->printExp();
    printf(")");
  } break;
  case ASTNUM:
    printf("%d", std::get<int>(this->variant));
    break;
  case ASTBOOL:
    if (std::get<int>(this->variant))
      printf("TRUE");
    else
      printf("FALSE");
    break;
  default:
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
  case ASTBAND:
  case ASTAND:
    printf("AND ");
    break;
  case ASTBOR:
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
  case ASTERROR:
    break;
  }
}
