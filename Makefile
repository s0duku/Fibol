DEF = -D FIBOL_FLATTEN
CC = clang++-12 -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1 -g $(DEF)
LEX = flex
GRAM = bison

LLVM_CONFIG = llvm-config-12
LLVM_CXXFLAGS += `$(LLVM_CONFIG) --cxxflags`
LLVM_LDFLAGS := `$(LLVM_CONFIG) --ldflags`
LLVM_LIBS = `$(LLVM_CONFIG) --system-libs --libs all bitwriter core support`

LOG_SRC = log.cc
LOG_OBJECT = objects/log.o 

TOOL = bin/fibol 

TOKENS_H = include/tokens.h

LEX_SRC = lexer/lexer.l
LEXER_SRC = lex.yy.c
LEXER_OBJECT = objects/lexer.o 

GRAM_SRC = parser/parser.y
PARSER_SRC = parser.tab.c
PARSER_OBJECT = objects/parser.o

TOOL_SRC = main.cc
TOOL_OBJECT = objects/tool.o 


EXP_AST_SRC = ast/ExpressionAST.cc
EXP_AST_OBJECT = objects/expAST.o

STMT_AST_SRC = ast/StatementAST.cc
STMT_AST_OBJECT = objects/stmtAST.o

IR_SRC = ast/IRGen.cc
IR_OBJECT = objects/IRGen.o

PASS_HELLO_SRC = passes/Hello.cc
PASS_HELLO_SO = lib/PASSHello.so

PASS_FLATTEN_SRC = passes/flatten.cc
PASS_FLATTEN_SO = lib/flatten.so
PASS_FLATTEN_OBJECT = objects/flatten.o

ASM_SRC = asm/Asm.cc
ASM_OBJECT = objects/Asm.o


INCLUDE_PATH = `pwd`


$(TOOL) : $(PASS_FLATTEN_OBJECT) $(PASS_FLATTEN_SO) $(ASM_OBJECT) $(PASS_HELLO_SO) $(TOOL_OBJECT) $(LEXER_OBJECT) $(LOG_OBJECT) $(PARSER_OBJECT) $(EXP_AST_OBJECT) $(STMT_AST_OBJECT) $(IR_OBJECT) testlexer
	$(CC) $(LLVM_CXXFLAGS) $(LLVM_LIBS) $(LLVM_LDFLAGS) $(PASS_FLATTEN_OBJECT) $(ASM_OBJECT) $(TOOL_OBJECT) $(IR_OBJECT) $(LEXER_OBJECT) $(LOG_OBJECT) $(PARSER_OBJECT) $(EXP_AST_OBJECT) $(STMT_AST_OBJECT) -o $(TOOL) -lm


testlexer.o: testlexer.cc
	$(CC) -c testlexer.cc -o objects/testlexer.o -I $(INCLUDE_PATH)

testlexer : testlexer.o $(LEXER_OBJECT) $(LOG_OBJECT)
	$(CC) $(LLVM_CXXFLAGS) $(LLVM_LIBS) $(LLVM_LDFLAGS) objects/testlexer.o $(LEXER_OBJECT) $(LOG_OBJECT) -o bin/testlexer -lm -I $(INCLUDE_PATH)


$(ASM_OBJECT) : $(ASM_SRC)
	$(CC) -c $(ASM_SRC) -o $(ASM_OBJECT) -I $(INCLUDE_PATH)

$(PASS_FLATTEN_SO) : $(PASS_FLATTEN_SRC)
	$(CC) -fPIC -shared $(LLVM_CXXFLAGS) $(LLVM_LIBS) $(LLVM_LDFLAGS) $(PASS_FLATTEN_SRC) -o $(PASS_FLATTEN_SO) -lm -I $(INCLUDE_PATH)

$(PASS_FLATTEN_OBJECT) : $(PASS_FLATTEN_SRC)
	$(CC) -fPIC -shared $(LLVM_CXXFLAGS) $(LLVM_LIBS) $(LLVM_LDFLAGS) $(PASS_FLATTEN_SRC) -o $(PASS_FLATTEN_OBJECT) -lm -I $(INCLUDE_PATH)

$(PASS_HELLO_SO) : $(PASS_HELLO_SRC)
	$(CC) -fPIC -shared $(LLVM_CXXFLAGS) $(LLVM_LIBS) $(LLVM_LDFLAGS) $(PASS_HELLO_SRC) -o $(PASS_HELLO_SO) -lm -I $(INCLUDE_PATH)

$(IR_OBJECT) : $(IR_SRC)
	$(CC) -c $(IR_SRC) -o $(IR_OBJECT) -I $(INCLUDE_PATH)


$(EXP_AST_OBJECT) : $(EXP_AST_SRC)
	$(CC) -c $(EXP_AST_SRC) -o $(EXP_AST_OBJECT) -I $(INCLUDE_PATH)

$(STMT_AST_OBJECT) : $(STMT_AST_SRC)
	$(CC) -c $(STMT_AST_SRC) -o $(STMT_AST_OBJECT) -I $(INCLUDE_PATH)

$(PARSER_SRC) : $(GRAM_SRC)
	$(GRAM) -Wconflicts-sr --defines=$(TOKENS_H) $(GRAM_SRC)

$(PARSER_OBJECT) : $(PARSER_SRC)
	$(CC) -c $(PARSER_SRC) -o $(PARSER_OBJECT) -I $(INCLUDE_PATH)

$(LOG_OBJECT) : $(LOG_SRC) $(PARSER_SRC)
	$(CC) -c $(LOG_SRC) -o $(LOG_OBJECT) -I $(INCLUDE_PATH)

$(LEXER_OBJECT) : $(LEXER_SRC) $(PARSER_SRC)
	$(CC) -c $(LEXER_SRC) -o $(LEXER_OBJECT) -I $(INCLUDE_PATH)
 
$(LEXER_SRC) : $(LEX_SRC) $(PARSER_SRC)
	$(LEX) $(LEX_SRC)

$(TOOL_OBJECT) : $(TOOL_SRC) $(PARSER_SRC)
	$(CC) -c $(TOOL_SRC) -o $(TOOL_OBJECT) -I $(INCLUDE_PATH)

clean :
	rm -rf $(PARSER_SRC) $(PARSER_OBJECT) bin/testlexer objects/testlexer.o
	rm -rf $(LEXER_SRC) $(TOOL_OBJECT) $(LOG_OBJECT) $(EXP_AST_OBJECT)
	rm -rf $(LEXER_SRC) $(LEXER_OBJECT) $(TOOL) $(STMT_AST_OBJECT)
	rm -rf $(IR_OBJECT) $(PASS_HELLO_SO) $(ASM_OBJECT) $(PASS_FLATTEN_OBJECT)

