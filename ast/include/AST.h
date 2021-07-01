
#include<vector>
#include<string>

#include"llvm/IR/DerivedTypes.h"

enum{
        NumberAST_INT,
        NumberAST_REAL
 };

enum PrimaryExpressionType{
        PrimaryExpressionAST_VARIABLE,
        PrimaryExpressionAST_NUMBER,
        PrimaryExpressionAST_FUNCTION,
        PrimaryExpressionAST_EXPRESSION
};

enum{
        ExpressionAST_ASSIGN,
        ExpressionAST_FUNCTION,
        ExpressionAST_CONDITIONAL
};

enum{
        StatementAST_RETURN,
        StatementAST_SELECTION,
        StatementAST_EXPRESSION,
        StatementAST_COMPOUND
};

class BaseAST;
class ProgramAST;
class StatementAST;
class StatementListAST;
class CompoundStatementAST;
class ExpressionStatementAST;
class ExpressionAST;
class AssignExpressionAST;
class ConditionalExpressionAST;
class ParametersAST;
class FunctionExpressionAST;
class AdditiveExpressionAST;
class MultiplicativeExpressionAST;
class PrimaryExpressionAST;
class VariableAST;
class NumberAST;
class FunctionCallAST;
class ArgumentsAST;
class ReturnStatementAST;
class SelectionStatementAST;

class BaseAST {
    public:
    virtual ~BaseAST() = default;
    virtual llvm::Value *codegen();
};

class ProgramAST:public BaseAST{
    public:
    StatementListAST *stmts;
    ProgramAST(StatementListAST *);
    llvm::Value *codegen() override;
};

class StatementAST:public BaseAST{
    public:
    int type;
    union{
        ReturnStatementAST * ret_stmt;
        CompoundStatementAST * compound_stmt;
        ExpressionStatementAST * exp_stmt;
        SelectionStatementAST * selection_stmt;
    }val;
    StatementAST();
    StatementAST(ReturnStatementAST *);
    StatementAST(CompoundStatementAST *);
    StatementAST(SelectionStatementAST *);
    StatementAST(ExpressionStatementAST *);
    llvm::Value *codegen() override;
};

class StatementListAST:public BaseAST{
    public:
    std::vector<StatementAST *> statements;
    virtual void append(StatementAST *);
};


class ReturnStatementAST:public StatementAST{
    public:
    ExpressionAST * exp;
    ReturnStatementAST(ExpressionAST *);
    llvm::Value *codegen() override;
};


class SelectionStatementAST:public StatementAST{
    public:
    ExpressionAST * exp;
    StatementAST * statement_true;
    StatementAST * statement_false;
    SelectionStatementAST(ExpressionAST *,StatementAST *,StatementAST *);
    llvm::Value *codegen() override;
};

class CompoundStatementAST:public StatementAST{
    public:
    StatementListAST * statementlist;
    CompoundStatementAST(StatementListAST *);
    llvm::Value *codegen() override;
};

class ExpressionStatementAST:public StatementAST{
    public:
    ExpressionAST * exp;
    ExpressionStatementAST(ExpressionAST *);
    llvm::Value *codegen() override;
};

class ExpressionAST:public BaseAST{
    public:
    int type;
    union{
        AssignExpressionAST * assign_exp;
        FunctionExpressionAST * function_exp;
        ConditionalExpressionAST * conditional_exp;
    }val;
    ExpressionAST();
    ExpressionAST(AssignExpressionAST *);
    ExpressionAST(FunctionExpressionAST *);
    ExpressionAST(ConditionalExpressionAST *);
    llvm::Value *codegen() override;
};

class AssignExpressionAST:public ExpressionAST{
    public:
    VariableAST * var;
    ExpressionAST * exp;
    AssignExpressionAST(VariableAST *,ExpressionAST * );
    llvm::Value *codegen() override;
};

class ConditionalExpressionAST:public ExpressionAST{
    public:
    AdditiveExpressionAST * left;
    int op;
    AdditiveExpressionAST * right;
    ConditionalExpressionAST(AdditiveExpressionAST *,int,AdditiveExpressionAST *);
    llvm::Value *codegen() override;
};

class ParametersAST:public BaseAST{
    public:
    std::vector<std::string> ids;
    virtual void append(std::string);
};

class FunctionExpressionAST:public ExpressionAST{
    public:
    std::string id;
    ParametersAST * params;
    CompoundStatementAST * compound;
    FunctionExpressionAST(std::string,ParametersAST *,CompoundStatementAST *);
    llvm::Value *codegen() override;
};

class AdditiveExpressionAST:public ExpressionAST{
    public:
    AdditiveExpressionAST * left;
    int op;
    MultiplicativeExpressionAST * right;
    AdditiveExpressionAST(AdditiveExpressionAST*,int,MultiplicativeExpressionAST*);
    llvm::Value *codegen() override;
};

class MultiplicativeExpressionAST:public ExpressionAST{
    public:
    MultiplicativeExpressionAST * left;
    int op;
    PrimaryExpressionAST * right;
    MultiplicativeExpressionAST(MultiplicativeExpressionAST*,int,PrimaryExpressionAST *);
    llvm::Value *codegen() override;
};

class PrimaryExpressionAST:public ExpressionAST{
    public:
    int type;
    union{
        VariableAST * var;
        NumberAST * num;
        FunctionCallAST * call;
        ExpressionAST * exp;
    }val;
    PrimaryExpressionAST(VariableAST *);
    PrimaryExpressionAST(NumberAST *);
    PrimaryExpressionAST(FunctionCallAST *);
    PrimaryExpressionAST(ExpressionAST *);
    llvm::Value *codegen() override;
};

class VariableAST:public ExpressionAST{
    public:
    std::string id;
    std::vector<std::string> id_list;
    VariableAST(std::string);
    virtual void append(std::string);
    llvm::Value *codegen() override;
};

#define NUMBER_INT long long
#define NUMBER_REAL double

class NumberAST:public ExpressionAST{
    public:
    int type;
    union {
        NUMBER_INT int_val;
        NUMBER_REAL real_val;
    }val;
    NumberAST(NUMBER_INT);
    NumberAST(NUMBER_REAL);
    llvm::Value *codegen() override;
};

class FunctionCallAST:public ExpressionAST{
    public:
    VariableAST * var;
    ArgumentsAST * args;
    FunctionCallAST(VariableAST * ,ArgumentsAST *);
    llvm::Value *codegen() override;
};

class ArgumentsAST:public BaseAST{
    public:
    std::vector<ExpressionAST *> expressions;
    virtual void append(ExpressionAST *);

};