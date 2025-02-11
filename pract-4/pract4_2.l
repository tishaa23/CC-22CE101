%option noyywrap
%{
	#include<stdio.h>
%}

%%
([Cc][hH][Aa][Rr][Uu][Ss][Aa][Tt]) {printf("University");}
\n {printf("%s",yytext); return 0;}
%%

int main()
{
	printf("Enter String : ");
	yylex();
	return 0;
}
