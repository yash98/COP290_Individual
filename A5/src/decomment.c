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
				if (!(dqo || sqo)) co = 1;
			case comment_end:
				if (!(dqo || sqo)) co = 0;
			case double_qoute:
				if (!(co || sqo)) dqo = !(dqo);
			case single_qoute:
				if (!(co || dqo)) sqo = !(sqo);
		}
	
		token = yylex();
	}

	return exit_success;
}