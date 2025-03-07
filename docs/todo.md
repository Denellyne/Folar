# To Do

## Lexer
    
    Char-by-Char Lexer
    Tokens:

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
                lBracket : {
                rBracket : }
                lcBracket : (
                rcBracket : )
                dots : :                

        declarations/keywords:
            if
            while
            let : makes const variable
            mut : makes variable mutable
            {type} :  specifies variable/function type
            # : preprocessor macro
            return : return from function
            
        preprocessor:
            include <{library/libraryPath}> : includes library
        
        types:
            int8
            int16
            int32
            int64
            uint8
            uint16
            uint32
            uint64
            
            char
            uchar
            
            string

            bool
            
            float32
            float64
            float128

            void
            

   
