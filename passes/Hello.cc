#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"  

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"


using namespace llvm;

namespace{
    class Hello : public FunctionPass{
        public:
        static char ID;
        Hello() : FunctionPass(ID){}
    
    bool runOnFunction(Function &f){
        errs() << "Hello Pass: function ";
        errs().write_escaped(f.getName()) << "\n";
        //f.getBasicBlockList().end()->getTerminator();
        return false;
    }

    };
}

char Hello::ID = 0;

static RegisterPass<Hello> X("hello", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);  

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new Hello()); });