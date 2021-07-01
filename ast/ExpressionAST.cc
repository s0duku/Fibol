#include"include/AST.h"

ExpressionAST::ExpressionAST(){;}

ExpressionAST::ExpressionAST(AssignExpressionAST * exp){
    this->type = ExpressionAST_ASSIGN;
    this->val.assign_exp = exp;
}

ExpressionAST::ExpressionAST(FunctionExpressionAST * exp){
    this->type = ExpressionAST_FUNCTION;
    this->val.function_exp = exp;
}

ExpressionAST::ExpressionAST(ConditionalExpressionAST * exp){
    this->type = ExpressionAST_CONDITIONAL;
    this->val.conditional_exp = exp;
}

AssignExpressionAST::AssignExpressionAST(VariableAST *var,ExpressionAST * exp){
    this->var = var;
    this->exp = exp;
}

ConditionalExpressionAST::ConditionalExpressionAST(AdditiveExpressionAST * left,int op,AdditiveExpressionAST * right){
    this->op = op;
    this->left = left;
    this->right = right;
}

void ParametersAST::append(std::string id){
    this->ids.push_back(id);
}

FunctionExpressionAST::FunctionExpressionAST(std::string id,ParametersAST * params,CompoundStatementAST * compound_statement){
    this->id = id;
    this->params = params;
    this->compound = compound_statement;
}

AdditiveExpressionAST::AdditiveExpressionAST(AdditiveExpressionAST*left,int op ,MultiplicativeExpressionAST* right){
    this->op = op;
    this->left = left;
    this->right = right;
}

MultiplicativeExpressionAST::MultiplicativeExpressionAST(MultiplicativeExpressionAST*mul_exp,int op,PrimaryExpressionAST *p_exp){
    this->op = op;
    this->left = mul_exp;
    this->right = p_exp;
}

PrimaryExpressionAST::PrimaryExpressionAST(VariableAST *var){
    this->type = PrimaryExpressionAST_VARIABLE;
    this->val.var = var;
}

PrimaryExpressionAST::PrimaryExpressionAST(NumberAST *num){
    this->type = PrimaryExpressionAST_NUMBER;
    this->val.num = num;
}

PrimaryExpressionAST::PrimaryExpressionAST(FunctionCallAST *fcall){
    this->type = PrimaryExpressionAST_FUNCTION;
    this->val.call = fcall;
}

PrimaryExpressionAST::PrimaryExpressionAST(ExpressionAST *exp){
    this->type = PrimaryExpressionAST_EXPRESSION;
    this->val.exp = exp;
}

VariableAST::VariableAST(std::string id){
    this->id = id;
}

void VariableAST::append(std::string id){
    this->id_list.push_back(id);
}

NumberAST::NumberAST(NUMBER_INT val){
    this->type = NumberAST_INT;
    this->val.int_val = val;
}

NumberAST::NumberAST(NUMBER_REAL val){
    this->type = NumberAST_REAL;
    this->val.real_val = val;
}

FunctionCallAST::FunctionCallAST(VariableAST * var,ArgumentsAST *args){
    this->var = var;
    this->args = args;
}

void ArgumentsAST::append(ExpressionAST *exp){
    this->expressions.push_back(exp);
}