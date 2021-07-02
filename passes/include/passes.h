#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"

class Flatten:public llvm::FunctionPass{
        public:
        static char ID;
        Flatten();
        
        bool flatten(llvm::Function *f);
        bool runOnFunction(llvm::Function &F);
};