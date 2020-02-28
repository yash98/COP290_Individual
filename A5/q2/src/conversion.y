%{
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct retS{
	int num;
	char * str;
} ret;

void stringify(ret expr, ret val);
void operator(ret expr, ret left, ret left, char o);

int place = 0;

%}

%union{char oper; ret r;}
%start line 
%token <r> number
%type <r> exp line
%token <oper> op

%%

line : exp '\n'	{printf("%s\n",$1);}

exp :	number	{stringify($$, $1);}
	|	exp exp op	{operator($$, $1, $2, $3);}
	;

%%

int main() {
	yyparse();

	return 0;
}

void stringify(ret expr, ret val) {
	expr.num = val.num;
	expr.str = malloc(11*sizeof(char));
	sprintf(expr.str, "%d\0", expr.num);
}

void operator(ret expr, ret left, ret left, char o) {
	expr.str = malloc((1+1+strlen(left.str)+1+strlen(right.str)+1)*sizeof(char));
	switch(o) {
		case '+':
			expr.num = right.num + left.num;
			break;
		case '-':
			expr.num = right.num + left.num;
			break;
		case '*':
			expr.num = right.num + left.num;
			break;
		case '/':
			expr.num = right.num + left.num;
			break;
		default:
			printf("invalid_input\n");
			exit(0);
	}
	char * w;
	*w = op;
	w += 1;
	*w = ' ';
	w += 1;
	strcpy(w, left.str);
	w += strlen(left.str);
	*w = ' ';
	strcpy(w, right.str);
	free(left.str);
	free(right.str);
}
