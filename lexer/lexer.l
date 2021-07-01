%option noyywrap
%x COMMENT_STATE

%{

#include "ast/include/AST.h"
#include "include/tokens.h"
#include<stdio.h>
#include<stdlib.h>

int num = 0;
int lexer_yyline = 1;
int lexer_yychar = 1;
int lexer_total_char = 0;

%}


PLUS        "+"
MINUS       "-"
STAR        "*"
DIV         "/"
ASSIGN      "="
EQUAL       "=="
NEQUAL      "!="
NOT         "!"
GREATER     ">"
LESSER      "<"
LBRACKET    "("
RBRACKET    ")"
AND         "&&"
OR          "||"
DOT         "."
COMMA       ","
LBRACE      "{"
RBRACE      "}"
SEMICOLON   ";"

INT         ([0-9]+)|(0x[0-9a-fA-F]+)
REAL        {INT}+"."{INT}+
ID          [_a-zA-Z][_a-zA-Z0-9]*
IF          if
ELSE        else 
RETURN      return 
FUNCTION    function        
    

%%

<COMMENT_STATE>{
<<EOF>>         {BEGIN(INITIAL);return TOKENS_EOF;}
"*/"            {lexer_total_char += yyleng;lexer_yychar += yyleng;BEGIN(INITIAL);}
\n              {++lexer_total_char;++lexer_yyline;lexer_yychar = 1;}
[ \t]+          {lexer_total_char += yyleng;lexer_yychar += yyleng;}
.+              {lexer_total_char += yyleng;lexer_yychar += yyleng;}
}

"/*"            {lexer_total_char += yyleng;lexer_yychar += yyleng;BEGIN(COMMENT_STATE);}
"//".*          {lexer_total_char += yyleng;lexer_yychar += yyleng;}
\n              {++lexer_total_char;++lexer_yyline;lexer_yychar = 1;}
[ \t]+          {lexer_total_char += yyleng;lexer_yychar += yyleng;}



{IF}            {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_IF;}
{ELSE}          {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_ELSE;}
{RETURN}        {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_RETURN;}
{FUNCTION}      {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_FUNCTION;}
{ID}            {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_ID;}
{PLUS}          {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_PLUS;}
{MINUS}         {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_MINUS;}
{STAR}          {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_STAR;}
{DIV}           {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_DIV;}
{LBRACKET}      {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_LBRACKET;}
{RBRACKET}      {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_RBRACKET;}
{ASSIGN}        {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_ASSIGN;}
{EQUAL}         {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_EQUAL;}
{SEMICOLON}     {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_SEMICOLON;}
{LBRACE}        {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_LBRACE;}
{RBRACE}        {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_RBRACE;}
{NEQUAL}        {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_NEQUAL;}
{NOT}           {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_NOT;}
{AND}           {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_AND;}
{OR}            {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_OR;}
{GREATER}       {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_GREATER;}
{LESSER}        {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_LESSER;}
{DOT}           {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_DOT;}
{COMMA}         {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_COMMA;}
{INT}           {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_INT;}
{REAL}          {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_REAL;}

.               {lexer_total_char += yyleng;lexer_yychar += yyleng;return TOKENS_ERROR;}

<<EOF>>         {return TOKENS_EOF;}

%%

char * lexer_text(){
    return yytext;
}

int lexer_leng(){
    return yyleng;
}

FILE * lexer_set_in(FILE * fp){
    yyin = fp;
    return yyin;
}

FILE * lexer_set_out(FILE *fp){
    yyout = fp;
    return yyout;
}

int lexer(){
    return yylex();
}

int lexer_iseof(int tok){
    return tok == TOKENS_EOF ? 1:0;
}

int lexer_iserr(int tok){
    return tok == TOKENS_ERROR ? 1:0;
}

int lexer_line(){
    return lexer_yyline;
}

int lexer_char(){
    return lexer_yychar;
}