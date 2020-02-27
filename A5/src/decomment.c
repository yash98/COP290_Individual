#include <stdio.h>
#include "decomment.h"

extern int yylex();
extern int yylineno;
extern char* yytext;

int main(void) 
{
	int token, dqo, sqo, co;
	
	token = yylex();

	while(token) {
		switch (token) {
			case comment_start:
				if (!(dqo || sqo)) {
					co = 1;
					fprintf(stdout, " ");
				} else fprintf(stdout, yytext);
			case comment_end:
				if (!(dqo || sqo)) co = 0;
				else fprintf(stdout, yytext);
			case double_qoute:
				if (!(co || sqo)) dqo = !(dqo);
				fprintf(stdout, yytext);
			case single_qoute:
				if (!(co || dqo)) sqo = !(sqo);
				fprintf(stdout, yytext);
			case nextline:
				fprintf(stdout, yytext);
			case rest:
				if (!co) fprintf(stdout, yytext);
			default:
				fprintf(stderr, "unknown string\n");
				return exit_failure;
		}
	
		token = yylex();
	}

	return exit_success;
}