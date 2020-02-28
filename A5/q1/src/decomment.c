#include <stdio.h>
#include "decomment.h"

extern int yylex();
extern int yylineno;
extern char* yytext;

int main(void) 
{
	int token, dqo = 0, sqo = 0, co = 0, con = -1;
	
	token = yylex();

	while(token) {
		switch (token) {
			case comment_start:
				if (!(dqo || sqo)) {
					co = 1;
					con = yylineno;
					fprintf(stdout, " ");
				} else fprintf(stdout, "%s", yytext);
				break;
			case comment_end:
				if (!(dqo || sqo || !co)) co = 0;
				else fprintf(stdout, "%s", yytext);
				break;
			case double_qoute:
				if (!(co || sqo)) dqo = !(dqo);
				fprintf(stdout, "%s", yytext);
				break;
			case single_qoute:
				if (!(co || dqo)) sqo = !(sqo);
				fprintf(stdout, "%s", yytext);
				break;
			case nextline:
				fprintf(stdout, "%s", yytext);
				break;
			case rest:
				if (!co) {
					fprintf(stdout, "%s", yytext);
				}
				break;
			default:
				fprintf(stderr, "unknown string: %s\n", yytext);
				return exit_failure;
		}

		// fflush(stdout);
		token = yylex();
	}

	if (co) {
		fprintf(stderr, "Error: line %d: unterminated comment\n", con);
		return exit_failure;
	}

	return exit_success;
}
