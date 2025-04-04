%{
    #include <stdio.h>
    #include <stdlib.h>
    
    int yylex();
    void yyerror(const char *s);
    int line_no = 1;
%}
    
    %token IF ELSE IDENTIFIER NUMBER RELOP BOOLEAN AND OR
    %left ELSE
    
    %%
    
    program:
        statements
        ;
    
    statements:
        | statements statement
        ;
    
    statement:
        IF '(' condition ')' statement ELSE statement
        | IF '(' condition ')' statement
        | '{' statements '}'
        | IDENTIFIER ';'
        ;
    
    condition:
      IDENTIFIER RELOP IDENTIFIER
    | IDENTIFIER RELOP NUMBER
    | NUMBER RELOP IDENTIFIER
    | NUMBER RELOP NUMBER
    | IDENTIFIER
    | NUMBER
    | BOOLEAN
    | '!' IDENTIFIER
    | '!' NUMBER
    | '!' BOOLEAN
    | condition AND condition
    | condition OR condition
    ;
    
    %%
    
    void yyerror(const char *s) {
        fprintf(stderr, "Syntax Error at line %d: %s\n", line_no, s);
    }
    
    int main() {
        printf("Enter C code with if-else statements :\n");
        yyparse();
        printf("Parsing completed successfully.\n");
        return 0;
    }