# Expressions

declarations ->  functionDeclaration{declarations*} | structDeclaration{declarations*}
functionDeclaration ->  type identifier (( {NOTHING} | (type identifier,)*(type identifier) )) {statements*}
statements -> compoundStm | assignStm  | ifStm | whileStm 
assignStm -> let identifier (: type = expression | mut : type (= expression)?) ; 
compoundStm -> statements(statements?)
ifStm -> if expression {statements} (else statements)?
whileStm -> while expression {statements}

    if variable is mutable but no expression is assigned to it, the compiler will throw an error if the user tries to use it without assigning it first;

expression -> BitwiseExp
BitwiseExp -> BitwiseExp & OrExp | BitwiseExp ^ OrExp | BitwiseExp | OrExp 
OrExp -> AndExp | OrExp || AndExp 
AndExp -> RelExp | AndExp && RelExp 
RelExp -> AddExp | AddExp == AddExp | AddExp != AddExp | AddExp >= AddExp |AddExp <= AddExp |AddExp > AddExp |AddExp < AddExp 
AddExp -> MultExp | AddExp + MultExp | AddExp - MultExp
MultExp -> PowExp | MultExp * PowExp | MultExp / PowExp | MultExp % PowExp
PowExp -> UnaryExp | UnaryExp ** PowExp
UnaryExp -> !UnaryExp | + primary | - primary | primary
primary -> number | string | true | false | null | identifier | identifier({expression}*) | (expression)
