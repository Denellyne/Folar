#include "ast.h"
#include <string_view>

exp *mkStringLiteral(const std::string_view stringLiteral) {
  exp *e = new exp();
  e->tag = ASTSTRLITERAL;
  e->str = stringLiteral;
  return e;
}

exp *mkFloat(const double v) {
  exp *e = new exp();
  e->tag = ASTFLOAT;
  e->val = v;
  return e;
}
exp *mkNum(const int v) {
  exp *e = new exp();
  e->tag = ASTNUM;
  e->num = v;
  return e;
}
exp *mkBinOp(exp *lExp, const astOp op, exp *rExp) {

  exp *e = new exp();
  e->tag = ASTBINOP;
  e->binop.left = lExp;
  e->binop.right = rExp;
  e->binop.op = op;
  return e;
}

exp *mkUnaryOp(exp *expr, const astOp op) {
  exp *e = new exp();
  e->tag = ASTUNARYOP;
  e->unaryop.op = op;
  e->unaryop.exp = expr;
  return e;
}

exp *mkId(const std::string_view id) {

  exp *e = new exp();
  e->tag = ASTID;
  e->id = id;
  return e;
}

exp *mkBool(const int b) {
  exp *e = new exp();
  e->tag = ASTBOOL;
  e->num = b;
  return e;
}

stm *mkCompound(stm *lStm, stm *rStm) {

  stm *ptr = new stm();
  ptr->tag = ASTCOMPOUND;
  ptr->compound.fst = lStm;
  ptr->compound.snd = rStm;
  return ptr;
}

stm *mkAssign(const std::string_view id, const int type, exp *exp) {

  stm *ptr = new stm();
  ptr->tag = ASTASSIGN;
  ptr->assign.id = id;
  if (!exp) {
    switch (type) {
    case ASTFLOAT:
      exp = mkFloat(0);
      break;
    case ASTNUM:
      exp = mkNum(0);
      break;
    case ASTSTRLITERAL:
      exp = mkStringLiteral("");
      break;
    case ASTBOOL:
      exp = mkBool(0);
      break;
    default:
      exp = nullptr;
      break;
    }
  }
  if (!exp)
    return nullptr;
  ptr->assign.expr = exp;

  if (type == -1)
    ptr->assign.type = exp->tag;
  else
    ptr->assign.type = type;
  return ptr;
}

stm *mkFuncCall(const std::string_view id, args *args) {

  stm *ptr = new stm();
  ptr->tag = ASTFUNCTION;
  ptr->function.args = args;

  ptr->function.id = id;
  return ptr;
}

stm *mkIf(exp *cond, stm *thenBranch, stm *elseifBranch, stm *elseBranch) {
  stm *ptr = new stm();
  ptr->tag = ASTIF;
  ptr->ifStmt.cond = cond;
  ptr->ifStmt.thenBranch = thenBranch;
  ptr->ifStmt.elseBranch = elseBranch;
  return ptr;
}
stm *mkWhile(exp *cond, stm *body) {
  stm *ptr = new stm();
  ptr->tag = ASTWHILE;
  ptr->whileStmt.cond = cond;
  ptr->whileStmt.body = body;
  return ptr;
}
args *mkArg(exp *expr) {
  args *ptr = new args();
  ptr->arg = expr;
  ptr->next = nullptr;
  return ptr;
}
args *appendArg(args *root, args *newArg) {
  args *head = root;
  while (head->next)
    head = head->next;

  head->next = newArg;
  return root;
}

func *mkFunc(const std::string_view id, const int returnValue, stm *args) {

  func *ptr = new func();
  ptr->id = id;
  ptr->returnValueTag = returnValue;
  ptr->args = args;

  return ptr;
}

void printStm(stm *ptr) {

  if (!ptr)
    return;
  switch (ptr->tag) {
  case ASTASSIGN:
    printf("(");
    printf("%s", ptr->assign.id.c_str());
    switch (ptr->assign.type) {
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
    if (ptr->assign.expr) {
      printf(" (");
      printExp(ptr->assign.expr);
      printf(")");
    }
    printf(")");

    break;
  case ASTCOMPOUND:
    printStm(ptr->compound.fst);
    printf(" ");
    printStm(ptr->compound.snd);
    break;
  case ASTFUNCTION: {
    printf("%s", ptr->function.id.c_str());
    printf("(");
    printArgs(ptr->function.args);
    printf(")");
  } break;
  case ASTIF: {

    int hasElse = 0;
    printf("IF");
    printf(" THEN ");

    if (ptr->ifStmt.elseBranch) {
      printf(" ELSE ");
      hasElse = 1;
    }

    printExp(ptr->ifStmt.cond);
    printf(" (");
    printStm(ptr->ifStmt.thenBranch);
    printf(")");

    if (hasElse) {
      printf(" (");
      printStm(ptr->ifStmt.elseBranch);
      printf(")");
    }
  } break;
  case ASTWHILE:
    printf("WHILE");
    printf(" DO ");
    printExp(ptr->whileStmt.cond);
    printf(" ");
    printStm(ptr->whileStmt.body);
    break;
  }
}

void printArgs(args *arg) {
  if (!arg)
    return;
  while (arg) {
    printExp(arg->arg);
    arg = arg->next;
    if (arg)
      printf(" ");
  }
}
void printExp(exp *ptr) {
  if (!ptr)
    return;

  switch (ptr->tag) {
  case ASTDOUBLE:
  case ASTFLOAT:
    printf("%f", ptr->val);
    break;
  case ASTID:
    printf("%s", ptr->id.c_str());
    break;
  case ASTBINOP:
    printf("(");
    printOp(ptr->binop.op);
    printExp(ptr->binop.left);
    printf(" ");
    printExp(ptr->binop.right);
    printf(")");
    break;
  case ASTUNARYOP:
    printf("(");
    printOp(ptr->unaryop.op);
    printExp(ptr->unaryop.exp);
    printf(")");
    break;
  case ASTSTRLITERAL:
    printf("%s", ptr->str.c_str());
    break;
  case ASTNUM:
    printf("%d", ptr->num);
    break;
  case ASTBOOL:
    if (ptr->num)
      printf("TRUE");
    else
      printf("FALSE");
    break;
  }
}

void printOp(astOp op) {
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
