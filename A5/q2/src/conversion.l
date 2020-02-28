%{
#include "y.tab.h"
void yyerror (const char *s);
int yylex();

%}
%%

[+ | - | * | /]	{yylval.oper = yytext[0]; return op;}
[0-9]+	{yylval.n = atoi(yytext); return number;}
[ ]+	;
[\n]	return yytext[0];
.		;

%%
int yywrap (void) {return 1;}
