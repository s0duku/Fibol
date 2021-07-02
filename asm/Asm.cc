
#include<string>
#include<stdio.h>

#include"include/Asm.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

static Target * Tar;
static TargetMachine * TarMach;

int init_ASM_module(Module * md){
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto targetTriple = sys::getDefaultTargetTriple();

    md->setTargetTriple(targetTriple);

   
    std::string Error;

    auto Tar = TargetRegistry::lookupTarget(targetTriple,Error);

    if(!Tar){
        errs()<<Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();

    TarMach = Tar->createTargetMachine(targetTriple, CPU, Features, opt, Reloc::PIC_);

     

    md->setDataLayout(TarMach->createDataLayout());
     
    return 0;
}

int init_ASM_object(std::string fname,Module *md){
    std::error_code EC;
    raw_fd_ostream dest(fname,EC,sys::fs::OF_None);

    if(EC){
        errs() << "Could not open file: " << EC.message();
        return 1;
    }
    
    legacy::PassManager pass;
    auto FileType = CGFT_ObjectFile;

    if (TarMach->addPassesToEmitFile(pass, dest, 0, FileType)) {
        errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    

    pass.run(*md);
    
    dest.flush();

    return 0;
}


