#include "ast.h"
#include "../ErrorHandler/errorHandler.h"
#include <print>
#include <string>
astToken convertToken(const tokenId tag) {
  astToken tk;
  switch (tag) {
  case INT8Token:
    tk.exp = ASTNUM;
    break;
  }
  return tk;
}
astOp convertOp(const token tk) {
  switch (tk.id) {
  case EQUALToken:
    return ASTEQ;
  case NOTEQUALToken:
    return ASTNEQ;
  case GREATEQUALToken:
    return ASTGE;
  case GREATToken:
    return ASTGT;
  case LESSToken:
    return ASTLT;
  case LESSEQUALToken:
    return ASTLE;
  case ADDToken:
    return ASTPLUS;
  case SUBToken:
    return ASTMINUS;
  case MODULUSToken:
    return ASTMODULUS;
  case MULTIPLYToken:
    return ASTTIMES;
  case DIVIDEToken:
    return ASTDIV;
  }
  return ASTERROR;
}

exp::exp(const token &tk) {
  constexpr auto toNum = [](const std::string_view str) {
    long long num = 0;
    bool negative = str[0] == '-';
    for (const auto &ch : str) {
      if (ch == '-')
        continue;
      else
        num = (num * 10) + (ch - '0');
    }
    if (negative)
      num *= -1;

    return num;
  };
  constexpr auto toFloat = [](const std::string_view str) {
    long double num = 0;
    bool negative = str[0] == '-';
    bool decimal = 0;
    long long decimalStep = 10;
    for (const auto &ch : str) {
      if (ch == '-')
        continue;
      else if (ch == '.') {
        decimal = true;
        decimalStep = 10;
      } else if (!decimal)
        num = (num * 10.0f) + (ch - '0');
      else {
        num = num + ((float)(ch - '0') / decimalStep);
        decimalStep *= 10;
      }
    }
    if (negative)
      num *= -1.0f;

    return num;
  };
  switch (tk.id) {

  // case NULLToken:
  case TRUEToken:
    this->tag = ASTBOOL;
    this->variant = 1;
    break;
  case FALSEToken:
    this->tag = ASTBOOL;
    this->variant = 0;
    break;
  case CHARLiteralToken:
    this->tag = ASTCHAR;
    this->variant = tk.literal;
    break;
  case STRINGLiteralToken:
    this->tag = ASTSTRLITERAL;
    this->variant = tk.literal;
    break;
  case NUMBERLiteralToken:
    this->tag = ASTNUM;
    this->variant = toNum(tk.literal);
    break;
  case FLOATLiteralToken:
    this->tag = ASTFLOAT;
    this->variant = toFloat(tk.literal);
    break;
  case IDENTIFIERToken:
    this->tag = ASTID;
    this->variant = tk.literal;
    break;
  default: {
    this->tag = ASTNOTYPE;
  } break;
  }
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

  std::println("{} {} ", this->id, stringify(this->returnValueTag));
  std::println("( ");
  this->args->printStm();
  std::println(") ");
  std::println("( ");
  this->stmt->printStm();
  std::println(") ");
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
    std::println("(");
    std::println("{}", ref.id.c_str());
    switch (ref.type) {
    case ASTFLOAT:
      std::println(" ASTFLOAT");
      break;
    case ASTID:
      std::println(" ASTID");
      break;
    case ASTNUM:
      std::println(" INTEGER");
      break;
    case ASTSTRLITERAL:
      std::println(" STRING");
      break;
    case ASTBOOL:
      std::println(" ASTBOOL");
      break;
    default:
      break;
    }
    if (ref.expr) {
      std::println(" (");
      ref.expr->printExp();
      std::println(")");
    }
    std::println(")");
  }

  break;
  case ASTCOMPOUND: {

    compound &ref = std::get<compound>(this->variant);
    if (ref.fst)
      ref.fst->printStm();
    if (ref.snd) {
      std::println(" ");
      ref.snd->printStm();
    }
  } break;
  case ASTFUNCTION: {
    function &ref = std::get<function>(this->variant);
    std::println("{}", ref.id.c_str());
    std::println("(");
    if (ref.arg)
      ref.arg->printArgs();
    std::println(")");
  } break;
  case ASTIF: {

    ifStmt &ref = std::get<ifStmt>(this->variant);
    int hasElse = 0;
    std::println("IF");
    std::println(" THEN ");
    if (ref.elseBranch) {
      std::println(" ELSE ");
      hasElse = 1;
    }

    if (ref.cond)
      ref.cond->printExp();

    std::println(" (");
    if (ref.thenBranch)
      ref.thenBranch->printStm();
    std::println(")");

    if (hasElse) {
      std::println(" (");
      ref.elseBranch->printStm();
      std::println(")");
    }
  } break;
  case ASTWHILE:
    std::println("WHILE");
    std::println(" DO ");

    whileStmt &ref = std::get<whileStmt>(this->variant);
    if (ref.cond)
      ref.cond->printExp();
    if (ref.body) {
      std::println(" ");
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
    std::println(" ");
  return this->next->printArgs();
}
void exp::printExp() {
  // if (!this)
  //   return;

  switch (this->tag) {
  case ASTDOUBLE:
  case ASTFLOAT:
    std::println("{}", std::get<long double>(this->variant));
    break;
  case ASTID:
  case ASTSTRLITERAL:
    std::println("{}", std::get<std::string>(this->variant).c_str());
    break;
  case ASTBINOP: {
    binOp &ref = std::get<binOp>(this->variant);
    std::println("(");
    printOp(ref.op);
    ref.left->printExp();
    std::println(" ");
    ref.right->printExp();
    std::println(")");
  } break;
  case ASTUNARYOP: {
    unaryOp &ref = std::get<unaryOp>(this->variant);
    std::println("(");
    printOp(ref.op);
    ref.expr->printExp();
    std::println(")");
  } break;
  case ASTNUM:
    std::println("{}", std::get<long long>(this->variant));
    break;
  case ASTBOOL:
    if ((bool)std::get<long long>(this->variant))
      std::println("TRUE");
    else
      std::println("FALSE");
    break;
  default:
    break;
  }
}

void printOp(const astOp op) {
  switch (op) {
  case ASTPOW:
    std::println("POW ");
    break;
  case ASTPLUS:
    std::println("PLUS ");
    break;
  case ASTMINUS:
    std::println("MINUS ");
    break;
  case ASTTIMES:
    std::println("MULT ");
    break;
  case ASTDIV:
    std::println("DIV ");
    break;
  case ASTBAND:
  case ASTAND:
    std::println("AND ");
    break;
  case ASTBOR:
  case ASTOR:
    std::println("OR ");
    break;
  case ASTNOT:
    std::println("NOT ");
    break;
  case ASTEQ:
    std::println("EQUAL ");
    break;
  case ASTNEQ:
    std::println("NOT EQUAL ");
    break;
  case ASTLT:
    std::println("LESSER ");
    break;
  case ASTGT:
    std::println("GREATER ");
    break;
  case ASTLE:
    std::println("LESS EQUAL ");
    break;
  case ASTGE:
    std::println("GREAT EQUAL ");
    break;
  case ASTXOR:
    std::println("XOR ");
    break;
  case ASTMODULUS:
    std::println("MODULUS ");
    break;
  case ASTERROR:
    break;
  }
}
