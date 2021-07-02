#include "include/passes.h"

using namespace llvm;



char Flatten::ID = 0;

static RegisterPass<Flatten> X("flatten", "Fibol Flatten Pass");

Flatten::Flatten():llvm::FunctionPass(ID){
    ;
}


bool Flatten::runOnFunction(Function &f){
    return this->flatten(&f);
}

bool Flatten::flatten(Function *f){
    std::vector<BasicBlock*> BB;
    for(BasicBlock &b: *f){
        BB.push_back(&b);
    }

    if(BB.size()<=1){
        return false;
    }    

    BB.erase(BB.begin());

    auto fbb = &f->front();

   if(isa<BranchInst>(fbb->getTerminator())){
       BranchInst * br = cast<BranchInst>(fbb->getTerminator());

       if(br->isConditional()){
           CmpInst *cmpInst = cast<CmpInst>(fbb->getTerminator()->getPrevNode());
           BasicBlock *newBB = fbb->splitBasicBlock(cmpInst,"newBB");
           BB.insert(BB.begin(),newBB);
       }
   }

   BasicBlock *loopEntry = BasicBlock::Create(f->getContext(), "loopEntry", f, fbb);
   BasicBlock *loopEnd = BasicBlock::Create(f->getContext(), "loopEnd", f, fbb);
   fbb->moveBefore(loopEntry);
   fbb->getTerminator()->eraseFromParent();
   srand(time(0));
   int randNumCase = rand();
   AllocaInst *swVarPtr = new AllocaInst(Type::getInt32Ty(f->getContext()), 0, "switchVar", fbb);
   new StoreInst(ConstantInt::get(Type::getInt32Ty(f->getContext()), randNumCase), swVarPtr, fbb);
   BranchInst::Create(loopEntry, fbb);
   
    LoadInst *swVar = new LoadInst(Type::getInt32Ty(f->getContext()), swVarPtr, "swVar", false, loopEntry);
    BranchInst::Create(loopEntry, loopEnd);

   BasicBlock *swDefault = BasicBlock::Create(f->getContext(), "swDefault", f, loopEnd);
   BranchInst::Create(loopEnd, swDefault);
   SwitchInst *swInst = SwitchInst::Create(swVar, swDefault, 0, loopEntry);

   for(BasicBlock *b:BB){
       ConstantInt *num = cast<ConstantInt>(ConstantInt::get(Type::getInt32Ty(f->getContext()), randNumCase));
        b->moveBefore(loopEnd);
        swInst->addCase(num,b);
        randNumCase = rand();
   }

   for(BasicBlock *b:BB){
       if(b->getTerminator()->getNumSuccessors() == 0){
            continue;
        }

        if(b->getTerminator()->getNumSuccessors() == 1){
        BasicBlock *sucBB = b->getTerminator()->getSuccessor(0);
        b->getTerminator()->eraseFromParent();
        ConstantInt *numCase = swInst->findCaseDest(sucBB);
        new StoreInst(numCase, swVarPtr, b);
        BranchInst::Create(loopEnd, b);
        continue;
    }

    if(b->getTerminator()->getNumSuccessors() == 2){
        ConstantInt *numCaseTrue = swInst->findCaseDest(b->getTerminator()->getSuccessor(0));
        ConstantInt *numCaseFalse = swInst->findCaseDest(b->getTerminator()->getSuccessor(1));
        BranchInst *br = cast<BranchInst>(b->getTerminator());
        SelectInst *sel = SelectInst::Create(br->getCondition(), numCaseTrue, numCaseFalse, "", b->getTerminator());
        b->getTerminator()->eraseFromParent();
        new StoreInst(sel, swVarPtr, b);
        BranchInst::Create(loopEnd, b);
    }
   }

    return true;
}