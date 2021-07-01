%{

#include<stdio.h>
#include<stdlib.h>
#include"include/lexer.h"
#include"include/utils.h"
#include"include/op_type.h"


int yylex();
void yyerror (char const *s);
ProgramAST *prog = 0;
std::vector<std::vector<std::string>> param_ids;
std::vector<std::vector<ExpressionAST *>> args_exps;
std::vector<std::vector<StatementAST *>> list_stmts;
std::vector<std::string> tmp_syms;

%}

%token TOKENS_EOF 0
    TOKENS_ID
    TOKENS_INT
    TOKENS_REAL
    TOKENS_IF
    TOKENS_ELSE
    TOKENS_RETURN
    TOKENS_PLUS
    TOKENS_STAR
    TOKENS_MINUS
    TOKENS_DIV
    TOKENS_LBRACKET
    TOKENS_RBRACKET
    TOKENS_ASSIGN
    TOKENS_SEMICOLON
    TOKENS_LBRACE
    TOKENS_RBRACE
    TOKENS_EQUAL
    TOKENS_NEQUAL
    TOKENS_NOT
    TOKENS_GREATER
    TOKENS_LESSER
    TOKENS_AND
    TOKENS_OR
    TOKENS_DOT
    TOKENS_COMMA
    TOKENS_FUNCTION
    TOKENS_ERROR


%union{
  BaseAST * ast;
  int op;
  std::string* id_sym;
}

%type <ast> 
primary_expression
program 
statement_list
statement
compound_statement
expression_statement
selection_statement
return_statement
expression
function_expression
conditional_expression
multiplicative_expression
additive_expression
function_call
number
variable
assign_expression
parameters
arguments
statements

%type <op> 
mul_op
add_op
relation_op

%type <id_sym>
parameter


%%

program:                {prog = 0;}statements  {prog = new ProgramAST((StatementListAST*)$2);}

statements:             {list_stmts.push_back(std::vector<StatementAST *>());} statement_list {$$ = new StatementListAST();((StatementListAST *)($$))->statements = list_stmts[list_stmts.size()-1];list_stmts.pop_back();}
                    |   %empty  {$$ = new StatementListAST();list_stmts.pop_back();}


statement_list:         statement_list statement {list_stmts[list_stmts.size()-1].push_back((StatementAST *)$2);}
                    |   statement  {list_stmts[list_stmts.size()-1].push_back((StatementAST *)$1);}

statement:              expression_statement  {$$ = new StatementAST((ExpressionStatementAST *)$1);}
                    |   return_statement  {$$ = new StatementAST((ReturnStatementAST *)$1);}
                    |   selection_statement {$$ = new StatementAST((SelectionStatementAST *)$1);}
                    |   compound_statement  {$$ = new StatementAST((CompoundStatementAST *)$1);}

compound_statement:     TOKENS_LBRACE statements TOKENS_RBRACE  {$$ = new CompoundStatementAST((StatementListAST *)$2);}

expression_statement:   expression {$$ = new ExpressionStatementAST((ExpressionAST *)$1);}

return_statement:       TOKENS_RETURN expression  {$$ = new ReturnStatementAST((ExpressionAST *)$2);}

selection_statement:    TOKENS_IF TOKENS_LBRACKET expression TOKENS_RBRACKET statement  {$$ = new SelectionStatementAST((ExpressionAST *)$3,(StatementAST *)$5,0);}
                    |   TOKENS_IF TOKENS_LBRACKET expression TOKENS_RBRACKET statement TOKENS_ELSE statement {$$ = new SelectionStatementAST((ExpressionAST *)$3,(StatementAST *)$5,(StatementAST *)$7);}

expression:             assign_expression {$$ = new ExpressionAST((AssignExpressionAST*)$1);}
                    |   conditional_expression  {$$ = new ExpressionAST((ConditionalExpressionAST *)$1);}
                    |   function_expression {$$ = new ExpressionAST((FunctionExpressionAST *)$1);}

function_expression:    TOKENS_FUNCTION TOKENS_ID {tmp_syms.push_back(std::string(lexer_text(),lexer_text()+strlen(lexer_text())));} TOKENS_LBRACKET parameters TOKENS_RBRACKET compound_statement {$$ = new FunctionExpressionAST(tmp_syms[tmp_syms.size()-1],(ParametersAST *)$5,(CompoundStatementAST *)$7);tmp_syms.pop_back();}

parameters:             {param_ids.push_back(std::vector<std::string>());} parameter_list {$$ = new ParametersAST(); ((ParametersAST *)($$))->ids = param_ids[param_ids.size()-1]; param_ids.pop_back();}
                    |   %empty {$$ = new ParametersAST(); param_ids.pop_back();}

parameter_list:         parameter_list TOKENS_COMMA parameter {param_ids[param_ids.size()-1].push_back(*$3);}
                    |   parameter {param_ids[param_ids.size()-1].push_back(*$1);}

parameter:              TOKENS_ID {$$ = new std::string(lexer_text(),lexer_text()+strlen(lexer_text()));}

assign_expression:      variable TOKENS_ASSIGN expression {$$ = new AssignExpressionAST((VariableAST *)($1),(ExpressionAST *)($3));}

variable:               TOKENS_ID {$$ = new VariableAST(std::string(lexer_text(),lexer_text()+strlen(lexer_text())));}
                    |   variable TOKENS_DOT TOKENS_ID {((VariableAST *)$1)->append(std::string(lexer_text(),lexer_text()+strlen(lexer_text())));}

conditional_expression: additive_expression {$$ = new ConditionalExpressionAST(0,0,(AdditiveExpressionAST *)($1));}
                    |   additive_expression relation_op additive_expression {$$ = new ConditionalExpressionAST((AdditiveExpressionAST *)($1),$2,(AdditiveExpressionAST *)($3));}

relation_op:            TOKENS_GREATER {$$ = OP_TYPE_GREATER;}
                    |   TOKENS_EQUAL  {$$ = OP_TYPE_EQUAL;}
                    |   TOKENS_NEQUAL {$$ = OP_TYPE_NEQUAL;}
                    |   TOKENS_LESSER {$$ = OP_TYPE_LESSER;}

additive_expression:    multiplicative_expression {$$ = new AdditiveExpressionAST(0,0,(MultiplicativeExpressionAST *)($1));}
                    |   additive_expression add_op multiplicative_expression {$$ = new AdditiveExpressionAST((AdditiveExpressionAST *)($1),$2,(MultiplicativeExpressionAST *)($3));}

add_op:                 TOKENS_PLUS {$$ = OP_TYPE_PLUS;}
                    |   TOKENS_MINUS  {$$ = OP_TYPE_MINUS;}

multiplicative_expression:  primary_expression  {$$ = new MultiplicativeExpressionAST(0,0,(PrimaryExpressionAST *)($1));}
                    |   multiplicative_expression mul_op primary_expression {$$ = new MultiplicativeExpressionAST((MultiplicativeExpressionAST *)($1),$2,(PrimaryExpressionAST *)($3));}

mul_op:                 TOKENS_STAR {$$ = OP_TYPE_STAR;}
                    |   TOKENS_DIV  {$$ = OP_TYPE_DIV;}

primary_expression:     variable    {$$ = new PrimaryExpressionAST((VariableAST*)($1));}
                    |   number      {$$ = new PrimaryExpressionAST((NumberAST*)($1));}
                    |   TOKENS_LBRACKET expression TOKENS_RBRACKET  {$$ = new PrimaryExpressionAST((ExpressionAST*)($2));}
                    |   function_call   {$$ = new PrimaryExpressionAST((FunctionCallAST*)($1));}

number:                 TOKENS_INT  {$$ = new NumberAST(atoll(lexer_text()));}
                    |   TOKENS_REAL {$$ = new NumberAST(atof(lexer_text()));}

function_call:          variable TOKENS_LBRACKET arguments TOKENS_RBRACKET {$$ = new FunctionCallAST((VariableAST*)$1,(ArgumentsAST*)$3);}
arguments:              {args_exps.push_back(std::vector<ExpressionAST *>());} argument-list {$$ = new ArgumentsAST(); ((ArgumentsAST*)($$))->expressions = args_exps[args_exps.size()-1]; args_exps.pop_back();}
                    |   %empty {$$ = new ArgumentsAST();args_exps.pop_back();}

argument-list:          argument-list TOKENS_COMMA expression {args_exps[args_exps.size()-1].push_back((ExpressionAST *)($3));}
                    |   expression {args_exps[args_exps.size()-1].push_back((ExpressionAST *)($1));}

%%

void yyerror (char const *s){
  printf("%s at line: %d, char: %d\n",s,lexer_line(),lexer_char());
} 

ProgramAST * get_ast(){
  return prog;
}