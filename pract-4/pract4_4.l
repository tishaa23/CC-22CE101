%option noyywrap
%{
    #include<stdio.h>
    int charCount = 0;
    int wordCount = 0;
    int lineCount = 0;
%}

/* Rule section */

%%
\n {charCount++; lineCount++; }
[ \t]+ {charCount+= yyleng; }
[a-zA-Z0-9]+  {wordCount++; charCount += yyleng;}
. {charCount++;}
%%

int main()
{
    printf("Enter the string: ");
    yylex(); 
    printf("Character:%d\nWords:%d\nLine:%d\n", charCount, wordCount, lineCount);
    return 0;
}