%{
#include "parser.tab.h"
extern int line_no;
%}

%%

"//".*                 ;     
"#".*                  ;     

"if"        { return IF; }
"else"      { return ELSE; }
"=="|"!="|"<"|">"|"<="|">="  { return RELOP; }
"!"         { return '!'; }
"true"      { return BOOLEAN; }
"false"     { return BOOLEAN; }
[a-zA-Z_][a-zA-Z0-9_]*    { return IDENTIFIER; }
[0-9]+                   { return NUMBER; }

"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
";"                     { return ';'; }
"&&"                    { return AND; }
"||"                    { return OR; }
[\n]                    { line_no++; }
[ \t\r]+                ;   
.                       { printf("Unexpected character: %s at line %d\n", yytext, line_no); }

%%

int yywrap() {
    return 1;
}
