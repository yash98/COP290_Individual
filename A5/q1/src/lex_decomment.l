%{

#include "decomment.h"

%}

%%

"/*"	return comment_start;
"*/"	return comment_end;
"\""	return double_qoute;
"\'"	return single_qoute;
"\n"	{yylineno++; return nextline;}
.		return rest;

%%