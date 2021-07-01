
#include<stdio.h>
int lexer();
char * lexer_text();
int lexer_leng();
FILE * lexer_set_in(FILE *);
FILE * lexer_set_out(FILE *);
int lexer_iseof(int);
int lexer_iserr(int);
int lexer_line();
int lexer_char();