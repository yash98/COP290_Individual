%{
#include "y.tab.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

void yyerror (const char *s);
int yylex();

// %code requires {struct ret {
// 		int num;
// 		char * str;
// 		char c;
// 	};
// }

// typedef struct retS {
// 	int num;
// 	char * str;
// } ret;

void stringify(char* expr, int val);
void operator(char* expr, char* right, char* left, char o);

int place = 0;

%}

// %union{char oper; struct {int num; char * str;} r; int n;}
%union{char* r; int n; char oper;}
%start line 
%token <n> number
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

void stringify(char * expr, int val) {
	// expr.num = val;
	expr = malloc(11*sizeof(char));
	sprintf(expr, "%d\0", val);
}

void operator(char * expr, char * right, char * left, char o) {
	expr = malloc((1+1+strlen(left)+1+strlen(right)+1)*sizeof(char));
	// switch(o) {
	// 	case '+':
	// 		expr.num = right.num + left.num;
	// 		break;
	// 	case '-':
	// 		expr.num = right.num + left.num;
	// 		break;
	// 	case '*':
	// 		expr.num = right.num + left.num;
	// 		break;
	// 	case '/':
	// 		expr.num = right.num + left.num;
	// 		break;
	// 	default:
	// 		printf("invalid_input\n");
	// }
	char * w;
	w = expr;
	*w = o;
	w += 1;
	*w = ' ';
	w += 1;
	strcpy(w, left);
	w += strlen(left);
	*w = ' ';
	strcpy(w, right);
	free(left);
	free(right);
}

void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }