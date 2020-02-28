%{
#include "y.tab.h"


%}
%%

[+ | - | * | /]	{yylval.oper = yytext[0]; return op;}
[0-9]+	{yylval.r.num = atoi(yytext); return number;}
[ ]+	;
[\n]	return yytext[0];
.		;

%%
