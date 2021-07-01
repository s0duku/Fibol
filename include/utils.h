#include<vector>
#include<cstring>
#include"../ast/include/AST.h"
#include"../include/tokens.h"
#include "llvm/IR/Module.h"

ProgramAST * get_ast();
void value_IR_dump();
void init_IR_module(char * md_name);
llvm::Module * get_IR_module();