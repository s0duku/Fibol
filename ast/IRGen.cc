#include"include/AST.h" 
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
//#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetSelect.h"

#include "include/op_type.h"

using namespace llvm;

static char module_name[256] = {0};

static LLVMContext TheContext;
static Module TheModule(module_name,TheContext);
static legacy::FunctionPassManager *TheFPM;
static IRBuilder<> Builder(TheContext);

Module * get_IR_module(){
    return &TheModule;
}

AllocaInst *create_block_entry_alloca(Function*f,std::string name){
    IRBuilder<> tmp(&f->getEntryBlock(),f->getEntryBlock().begin());
    return tmp.CreateAlloca(Type::getInt64Ty(TheContext),0,name.c_str());
}

class NameEnv{
    public:
        std::vector<std::map<std::string, AllocaInst *>> envs;

        NameEnv(){
            this->envs.push_back(std::map<std::string, AllocaInst *>());
        }

        AllocaInst * lookup(std::string sym){
            AllocaInst * v = 0;
            int idx = this->envs.size();
            for(int i = idx-1;i >= 0;i--){
                v = this->envs[i][sym];
                if(v)
                    return v;
            }
            return v;
        }

        void set(std::string sym,AllocaInst * v){
            this->envs[this->envs.size()-1][sym] = v; 
        }

        void append(std::map<std::string, AllocaInst *> val_map){
            this->envs.push_back(val_map);
        }

        std::map<std::string, AllocaInst *> pop(){
            std::map<std::string, AllocaInst *> m = this->envs[this->envs.size()-1];
            this->envs.pop_back();
            return m;
        }
};

static NameEnv NamedValues;


void init_IR_module(char * md_name){
    strcpy(module_name,md_name);
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    TheFPM = new legacy::FunctionPassManager(&TheModule);
    TheFPM->add(createAggressiveDCEPass());
    //TheFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
    //TheFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
    TheFPM->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
    //TheFPM->add(createCFGSimplificationPass());

    TheFPM->doInitialization();

    std::vector<Type *> Int64es(0,Type::getVoidTy(TheContext));
    FunctionType *ft = FunctionType::get(Type::getVoidTy(TheContext),Int64es,false);
    Function *f = Function::Create(ft,Function::ExternalLinkage,"main",&TheModule);

    BasicBlock * startBB = BasicBlock::Create(TheContext, "start",f);
    Builder.SetInsertPoint(startBB);
}


Value * BaseAST::codegen(){
    return 0;
}

Value * ProgramAST::codegen(){
    unsigned idx = this->stmts->statements.size();
    Value * v;
    for(int i = 0;i < idx;i++){
        v = this->stmts->statements[i]->codegen();
    }

    BasicBlock * thisBB = Builder.GetInsertBlock();
    if (thisBB->size() != 0 && thisBB->back().isTerminator()){
        exit(0);
    }

    return Builder.CreateRetVoid();    
}

Value * CompoundStatementAST::codegen(){
    unsigned idx = this->statementlist->statements.size();
    Value * ret = 0;
    //BasicBlock *BB = BasicBlock::Create(TheContext, "block",0);
    //Builder.SetInsertPoint(BB);
    for(int i = 0;i < idx;i++){
        this->statementlist->statements[i]->codegen();
    }
    //Builder.SetInsertPoint(BasicBlock::Create(TheContext, "block_out",Builder.GetInsertBlock()->getParent()));
    return ConstantFP::get(TheContext,APFloat(0.0));
}


Value * StatementAST::codegen(){
    switch (this->type){
        case StatementAST_RETURN:
            return this->val.ret_stmt->codegen();
        case StatementAST_COMPOUND:
            return this->val.compound_stmt->codegen();
        case StatementAST_EXPRESSION:
            return this->val.exp_stmt->codegen();
        case StatementAST_SELECTION:
            return this->val.selection_stmt->codegen();
        default:
            
            exit(0);
    }
    return 0;
}


Value * ReturnStatementAST::codegen(){
    if(this->exp == 0){
        return Builder.CreateRet(ConstantInt::get(TheContext,APInt(64,0,true)));
    }
    return Builder.CreateRet(this->exp->codegen());
}

Value * SelectionStatementAST::codegen(){

    BasicBlock *thisBB = 0;

    Value * CondV = this->exp->codegen();

    
    Function *f = Builder.GetInsertBlock()->getParent();


    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", f);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
    BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

    Value * Br = Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    Builder.SetInsertPoint(ThenBB);


    this->statement_true->codegen();

    thisBB = Builder.GetInsertBlock();
    if (thisBB->size() == 0 || !thisBB->back().isTerminator()){
        Builder.CreateBr(MergeBB);
    }

    f->getBasicBlockList().push_back(ElseBB);

    Builder.SetInsertPoint(ElseBB);


    if(this->statement_false){
        this->statement_false->codegen();
    }

    thisBB = Builder.GetInsertBlock();
    if (thisBB->size() == 0 || !thisBB->back().isTerminator()){
        Builder.CreateBr(MergeBB);
    }

    f->getBasicBlockList().push_back(MergeBB);

    Builder.SetInsertPoint(MergeBB);
    
    return Br;
}

Value * ExpressionStatementAST::codegen(){
    return this->exp->codegen();
}


Value * ExpressionAST::codegen(){
    switch (this->type){
        case ExpressionAST_ASSIGN:
            return this->val.assign_exp->codegen();
        case ExpressionAST_CONDITIONAL:
            return this->val.conditional_exp->codegen();
        case ExpressionAST_FUNCTION:
            return this->val.function_exp->codegen();
        default:
            
            exit(0);
    }
    return 0;
}

Value * FunctionExpressionAST::codegen(){
    std::vector<Type *> Int64es(this->params->ids.size(),Type::getInt64Ty(TheContext));
    
    FunctionType *ft = FunctionType::get(Type::getInt64Ty(TheContext),Int64es,false);

    Function *f = Function::Create(ft,Function::ExternalLinkage,this->id,&TheModule);



    unsigned Idx = 0;

    for (auto &Arg : f->args())
        Arg.setName(this->params->ids[Idx++]);

    BasicBlock *back = Builder.GetInsertBlock();
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", f);
    Builder.SetInsertPoint(BB);

    NamedValues.append(std::map<std::string, AllocaInst *>());
   
    for (auto &Arg : f->args()){
        AllocaInst *Alloca = create_block_entry_alloca(f, std::string(Arg.getName()));
        Builder.CreateStore(&Arg, Alloca);
        NamedValues.set(std::string(Arg.getName()),Alloca);
    }

    Value * ret = this->compound->codegen();

    if(!ret){
        f->eraseFromParent();
        return 0;
    }

    BasicBlock *thisBB = Builder.GetInsertBlock();
    if (thisBB->size() == 0 || !thisBB->back().isTerminator()){
        Builder.CreateRet(ConstantInt::get(TheContext,APInt(64,0,true)));
    }
    
    Builder.SetInsertPoint(back);
    NamedValues.pop();
    TheFPM->run(*f);

    return f;
}


Value * AssignExpressionAST::codegen(){
    AllocaInst * alloc = 0;
    if(NamedValues.lookup(this->var->id) == 0){
         alloc = create_block_entry_alloca(Builder.GetInsertBlock()->getParent(),this->var->id);
         NamedValues.set(this->var->id,alloc);
    }

    alloc = NamedValues.lookup(this->var->id);

    Value * val = this->exp->codegen();
    Builder.CreateStore(val,alloc);

    return val;
}

Value * ConditionalExpressionAST::codegen(){
   
    if(this->op == 0){
        return this->right->codegen();
    }


    Value * left = this->left->codegen();
    Value * right = this->right->codegen();

    switch(this->op){
        case OP_TYPE_GREATER:
            return Builder.CreateICmpUGT(left,right,"greater");
        case OP_TYPE_LESSER:
            return Builder.CreateICmpULT(left,right,"lesser");
        case OP_TYPE_EQUAL:
            return Builder.CreateICmpEQ(left,right,"equal");
        case OP_TYPE_NEQUAL:
            return Builder.CreateICmpNE(left,right,"nequal");
        default:
            
            exit(0);
    }
    return 0;
}


Value * AdditiveExpressionAST::codegen(){
    if(this->op == 0){
        return this->right->codegen();
    }

    Value * left = this->left->codegen();
    Value * right = this->right->codegen();

    switch(this->op){
        case OP_TYPE_PLUS:
            return Builder.CreateAdd(left,right,"addtmp");
        case OP_TYPE_MINUS:
            return Builder.CreateSub(left,right,"subtmp");
        default:
            
            exit(0);
    }
    return 0;
}


Value * MultiplicativeExpressionAST::codegen(){

    if(this->op == 0){
        return this->right->codegen();
    }

    Value * left = this->left->codegen();
    Value * right = this->right->codegen();

    switch(this->op){
        case OP_TYPE_STAR:
            return Builder.CreateMul(left,right,"multmp");
        case OP_TYPE_DIV:
            return Builder.CreateSDiv(left,right,"divtmp");
        default:
            
            exit(0);
    }
    return 0;
}

Value * FunctionCallAST::codegen(){
    Function *f = TheModule.getFunction(this->var->id);
    
    if(!f){
        exit(0);
    }
    
    if(f->arg_size()!=this->args->expressions.size()){
        exit(0);
    }

    std::vector<Value *> ArgsV;
    for (unsigned i = 0;i < this->args->expressions.size(); ++i) {
        ArgsV.push_back(this->args->expressions[i]->codegen());
        if (!ArgsV.back())
            exit(0);
    }

    return Builder.CreateCall(f, ArgsV, "calltmp");
}

Value * PrimaryExpressionAST::codegen(){
    switch(this->type){
        case PrimaryExpressionAST_EXPRESSION:
            return this->val.exp->codegen();
        case PrimaryExpressionAST_FUNCTION:
            return this->val.call->codegen();
        case PrimaryExpressionAST_VARIABLE:
            return this->val.var->codegen();
        case PrimaryExpressionAST_NUMBER:
            return this->val.num->codegen();
        default:
            
            exit(0);
    }
    
    return 0;
}


Value * NumberAST::codegen(){

    if(this->type == NumberAST_INT){   
        return ConstantInt::get(TheContext,APInt(64,this->val.int_val,true));
    }else if(this->type == NumberAST_REAL){
        return 0;
    }
    return 0;
}

Value * VariableAST::codegen(){
    AllocaInst *v = NamedValues.lookup(this->id);
    if(!v){
        
        exit(0);
    }
    return Builder.CreateLoad(v, this->id.c_str());
}


void value_IR_dump(){
    
    TheModule.print(outs(),0);
}
