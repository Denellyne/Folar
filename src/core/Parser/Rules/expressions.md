# Expressions

declarations -> expressions | variableExpressions | functionDeclaration
variableExpressions -> let identifier (: type = expression | mut : type (= expression)?) ; 
functionDeclaration ->  type identifier (( {NOTHING} | (type identifier,)*(type identifier) )) {declarations*}

    if variable is mutable but no expression is assigned to it, the compiler will zero initialize it;

expression -> assignment
assignment -> identifier = assignment | equality
equality -> comparison( (!= | ==) comparison)*
comparison -> term ( ( > | >= | < | <= )term )*
term -> factor( ( - | + ) factor)*
factor -> unary( (/ | * | %) unary)*
unary -> (! | -) unary | primary
primary -> number | string | true | false | null | identifier | (expression)
