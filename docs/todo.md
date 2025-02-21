# To Do

## Lexer
    
    Char-by-Char Lexer
    Tokens:
        loops:
            if
            while
        operators:
            bitwise:
                not : ~
                and : &
                or : |
                lShift : <<
                rShift : >>
            comparison:
                not : !
                and : &&
                or : ||
                xor : ^
                less : <
                great : >
                lEq : <=
                gEq : >=
                eq : ==
            operations:
                mult : *
                add : +
                sub : -
                div : /
                mod : %
                assign : =
                ptr : *
                ref : &
            misc:
                end of statement : ;
                startOfComment : -!
                endOfComment : !-
        declarations:
            let -> makes const variable
            mut -> makes variable mutable
            : {type} :  specifies variable type
            Example:
                let x : int = 3; -- Valid
                let y : int; -- Invalid
                x = 4; -- Invalid , all variables are const by default
                let mut z : char = 'a'; -- Mutable variable
                z = 'c'; -- Valid because z is mutable

   
