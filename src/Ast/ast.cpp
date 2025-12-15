#include "ast.h"
#include "../ErrorHandler/errorHandler.h"
#include <string_view>

exp::exp(const std::string_view str, const expType tag) {
  this->tag = tag;
  if (tag == ASTSTRLITERAL)
    this->str = str;
  else if (tag == ASTID)
    this->id = str;
  else
    errorHandler::getInstance().reportError(INVALIDAST, str);
}

exp::exp(const double v) {
  this->tag = ASTFLOAT;
  this->val = v;
}
exp::exp(const int v, const expType tag) {
  this->tag = tag;
  this->num = v;
}
exp::exp(exp *lExp, const astOp op, exp *rExp) {
  this->tag = ASTBINOP;
  this->binop.left = lExp;
  this->binop.right = rExp;
  this->binop.op = op;
}

exp::exp(exp *expr, const astOp op) {
  this->tag = ASTUNARYOP;
  this->unaryop.op = op;
  this->unaryop.exp = expr;
}
exp::~exp() {
  if (this->tag == ASTBINOP) {
    delete this->binop.left;
    delete this->binop.right;
  } else if (this->tag == ASTUNARYOP)
    delete this->unaryop.exp;
  delete this;
}

stm::stm(stm *lStm, stm *rStm) {

  this->tag = ASTCOMPOUND;
  this->compound.fst = lStm;
  this->compound.snd = rStm;
}

stm::stm(const std::string_view id, const int type, exp *exp) {

  this->tag = ASTASSIGN;
  this->assign.id = id;
  this->assign.expr = exp;

  if (type == -1)
    this->assign.type = exp->tag;
  else
    this->assign.type = type;
}

stm::stm(const std::string_view id, args *args) {

  this->tag = ASTFUNCTION;
  this->function.args = args;

  this->function.id = id;
}

stm::stm(exp *cond, stm *thenBranch, stm *elseBranch) {
  this->tag = ASTIF;
  this->ifStmt.cond = cond;
  this->ifStmt.thenBranch = thenBranch;
  this->ifStmt.elseBranch = elseBranch;
}
stm::stm(exp *cond, stm *body) {
  this->tag = ASTWHILE;
  this->whileStmt.cond = cond;
  this->whileStmt.body = body;
}
args::args(exp *expr) {
  this->arg = expr;
  this->next = nullptr;
}
void args::appendArg(args *newArg) {
  args *head = this;
  while (head->next)
    head = head->next;

  head->next = newArg;
}

func::func(const std::string_view id, const int returnValue, class stm *args) {
  this->id = id;
  this->returnValueTag = returnValue;
  this->args = args;
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
    this->function.args->printArgs();
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
    this->unaryop.exp->printExp();
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
