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
				} else fprintf(stdout, "%s\n", yytext);
				break;
			case comment_end:
				if (!(dqo || sqo || !co)) co = 0;
				else fprintf(stdout, "%s\n", yytext);
				break;
			case double_qoute:
				if (!(co || sqo)) dqo = !(dqo);
				fprintf(stdout, "%s\n", yytext);
				break;
			case single_qoute:
				if (!(co || dqo)) sqo = !(sqo);
				fprintf(stdout, "%s\n", yytext);
				break;
			case nextline:
				fprintf(stdout, "%s\n", yytext);
				break;
			case rest:
				if (!co) fprintf(stdout, "%s\n", yytext);
				break;
			default:
				fprintf(stderr, "unknown string\n");
				return exit_failure;
		}
	
		token = yylex();
	}

	return exit_success;
}