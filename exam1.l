%{
#include <stdio.h>
%}

%%
"/"([^]|\+[^/])\+"/";
"//".*;
.                             { putchar(yytext[0]); }
\n                            { putchar('\n'); }
%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    printf("Enter C program : ");
    yylex();
    return 0;
}
