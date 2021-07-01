#include<stdio.h>
#include<stdlib.h>
#include"include/lexer.h"
#include"include/log.h"

int main(int argc, char** argv) {
    int tok = 0;
    lexer_set_out(fopen("/dev/null", "w"));
    if (argc > 1) {
        if (!(lexer_set_in(fopen(argv[1], "r")))) {
            perror(argv[1]);
            return 1;
        }
    }

    while(!lexer_iseof(tok)){
        tok = lexer();
        if(lexer_iserr(tok)){
            printf("Fatal error! line: %d, char: %d, %s\n",lexer_line(),lexer_char(),lexer_text());
            break;
        }
        log_token(tok,lexer_text());
    }
    
    return 0;
}