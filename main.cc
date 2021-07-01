#include<stdlib.h>
#include "asm/include/Asm.h"
#include"include/lexer.h"
#include"include/utils.h"



int main(int argc, char** argv) {
    ProgramAST * ast = 0;
    int size = 0;
    llvm::Value *v = 0;

    lexer_set_out(fopen("/dev/null", "w"));

    if(argc > 1){
        init_IR_module(argv[1]);
    }else{
        puts("fatal error: no input file");
    }

    if (argc == 2) {
        if (!(lexer_set_in(fopen(argv[1], "r")))) {
            perror(argv[1]);
            return 1;
        }

        yyparse();
        ast = get_ast();

        if(!ast){
            puts("fatal error: source parse error");
            return 1;
        }
        ast->codegen();
        value_IR_dump();
    }else if(argc == 3){
        if (!(lexer_set_in(fopen(argv[2], "r")))) {
            perror(argv[1]);
            return 1;
        }

        yyparse();
        ast = get_ast();

        if(!ast){
            puts("fatal error: source parse error");
            return 1;
        }
        ast->codegen();

        if(!strcmp(argv[1],"-c")){
            llvm::Module * md = get_IR_module();
            init_ASM_module(md);
            init_ASM_object(std::string(argv[2])+std::string(".o"),md);
            llvm::outs()<<"object file: "<<std::string(argv[2])+std::string(".o")<<"\n";
        }
    }

    return 0;
}