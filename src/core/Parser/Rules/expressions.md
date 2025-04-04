# Expressions

declarations -> expressions | variableExpressions
variableExpressions -> let identifier (: type = expression | mut : type (= expression)?) ; 

    if variable is mutable but no expression is assigned to it, the compiler will initialize it;

expression -> equality
equality -> comparison( (!= | ==) comparison)*
comparison -> term ( ( > | >= | < | <= )term )*
term -> factor( ( - | + ) factor)*
factor -> unary( (/ | * | %) unary)*
unary -> (! | -) unary | primary
primary -> number | string | true | false | null | identifier | (expression)
