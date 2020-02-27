%{

#include <decomment.h>

%}

%%

"/*"	return comment_start;
"*/"	return comment_end;
"\""	return double_qoute;
"\'"	return single_qoute;

[ ]		return space;
.		return rest;

%%
