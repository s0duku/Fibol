#include"ast/include/AST.h"
#include"include/tokens.h"
#include<stdio.h>

void log_token(int token,char *text){
    printf("TOKENS ");
    switch(token){
        case TOKENS_ID:
            printf("ID");break;
        case TOKENS_INT:
            printf("INT");break;
        case TOKENS_REAL:
            printf("REAL");break;
        case TOKENS_IF:
            printf("IF");break;
        case TOKENS_ELSE:
            printf("ELSE");break;
        case TOKENS_RETURN:
            printf("RETURN");break;
        case TOKENS_PLUS:
            printf("PLUS");break;
        case TOKENS_STAR:
            printf("STAR");break;
        case TOKENS_MINUS:
            printf("MINUS");break;
        case TOKENS_DIV:
            printf("DIV");break;
        case TOKENS_DOT:
            printf("DOT");break;
        case TOKENS_COMMA:
            printf("COMMA");break;
        case TOKENS_AND:
            printf("AND");break;
        case TOKENS_OR:
            printf("OR");break;
        case TOKENS_EQUAL:
            printf("EQUAL");break;
        case TOKENS_NEQUAL:
            printf("NEQUAL");break;
        case TOKENS_FUNCTION:
            printf("FUNCTION");break;
        case TOKENS_LBRACKET:
            printf("LBRACKET");break;
        case TOKENS_RBRACKET:
            printf("RBRACKET");break;
        case TOKENS_ASSIGN:
            printf("ASSIGN");break;
        case TOKENS_SEMICOLON:
            printf("SEMICOLON");break;
        case TOKENS_LBRACE:
            printf("LBRACE");break;
        case TOKENS_RBRACE:
            printf("RBRACE");break;
        case TOKENS_GREATER:
            printf("GREATER");break;
        case TOKENS_LESSER:
            printf("LESSER");break;
        case TOKENS_ERROR:
            printf("ERROR");break;
        case TOKENS_EOF:
            printf("EOF\n");return;
        default:
            printf("UNKNOWN");break;
    }
    printf(" %s",text);
    printf("\n");
}