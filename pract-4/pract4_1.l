%option noyywrap
%{
	#include<stdio.h>
%}

%%
[^0-9\n]  ;
[0-9]+	{printf("%s\n",yytext);}
\n {return 0;}
%%

int main()
{
	printf("Enter String : ");
	yylex();
	return 0;
}
