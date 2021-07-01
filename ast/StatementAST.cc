#include"include/AST.h" 

StatementAST::StatementAST(){
    ;
}
StatementAST::StatementAST(ReturnStatementAST *stmt){
    this->type = StatementAST_RETURN;
    this->val.ret_stmt = stmt;
}
StatementAST::StatementAST(CompoundStatementAST *stmt){
    this->type = StatementAST_COMPOUND;
    this->val.compound_stmt = stmt;
}
StatementAST::StatementAST(SelectionStatementAST *stmt){
    this->type = StatementAST_SELECTION;
    this->val.selection_stmt = stmt;
}
StatementAST::StatementAST(ExpressionStatementAST *stmt){
    this->type = StatementAST_EXPRESSION;
    this->val.exp_stmt = stmt;
}

ReturnStatementAST::ReturnStatementAST(ExpressionAST *exp){
    this->exp = exp;
}

SelectionStatementAST::SelectionStatementAST(ExpressionAST *exp,StatementAST *stmt_true,StatementAST *stmt_false){
    this->exp = exp;
    this->statement_true = stmt_true;
    this->statement_false = stmt_false;
}

CompoundStatementAST::CompoundStatementAST(StatementListAST *stmt_list){
    this->statementlist = stmt_list;
}

ExpressionStatementAST::ExpressionStatementAST(ExpressionAST * exp){
    this->exp = exp;
}

ProgramAST::ProgramAST(StatementListAST *stmts){
    this->stmts = stmts;
}

void StatementListAST::append(StatementAST * stmt){
    this->statements.push_back(stmt);
}
