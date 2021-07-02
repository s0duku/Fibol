# Fibol A Compiler Based On Flex/Bison/LLVM  

Design a simple Language just for learning these tools ;) 

![helloworld_example](https://gitee.com/s0duku/Fibol/raw/master/examples/run_example.png "在这里输入图片标题")

#### Learning resources  

[LLVM OFFICIAL https://llvm.org/docs/GettingStartedTutorials.html](https://llvm.org/docs/GettingStartedTutorials.html)    
[Bison DOCS http://dinosaur.compilertools.net/bison/index.html](http://dinosaur.compilertools.net/bison/index.html)   
[Flex DOCS http://dinosaur.compilertools.net/flex/index.html](http://dinosaur.compilertools.net/flex/index.html)  


## WOKING ON ......  

Lexer       90% (build with Flex)    
Parser      90% (build with Bison)  
IRGenerator 80% (build with LLVM)  
JIT          0% (build with LLVM)  
Passes       10% (build with LLVM)    
Assembler    70% (build with LLVM)  
Standard Library 10% (build with C/C++)  
  
## INSTALLION

Depends on LLVM-12, clang++-12, before you start to compile, make sure you have installed LLVM-12, clang++-12  

```
git clone https://gitee.com/s0duku/Fibol.git  

cd Fibol  

./build.sh

```

## FEATURES  

* Function as the first class data type  
* Only two data type, Int64, Function  
* Every Function returns a Int64  

### usage

```
./bin/fibol -c helloworld.fbl

./bin/flink helloworld.fbl.o std/FibolStdio.o -o helloworld

./hellworld

```

```
Hello World For Fibol Language!
magic number is:
5050

```

## EXAMPLE  

### source file

```
//randomly write some code for test ;)


function fibonacci(n){
	if(n > 1)
		return	n + fibonacci(n-1)
	else
		return 1 
}


PrintOutLn("")

PrintOutLn("Hello World For Fibol Language!")

PrintOutLn("magic number is: ")

PrintOutI(fibonacci(100))

PrintOutLn("")


```
###  generate object file

```
./bin/fibol -c helloworld.fbl
```

### object file

```
Symbol table '.symtab' contains 11 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS
     2: 0000000000000000     1 OBJECT  LOCAL  DEFAULT    4 .L__unnamed_1
     3: 0000000000000001    32 OBJECT  LOCAL  DEFAULT    4 .L__unnamed_2
     4: 0000000000000021    18 OBJECT  LOCAL  DEFAULT    4 .L__unnamed_3
     5: 0000000000000033     1 OBJECT  LOCAL  DEFAULT    4 .L__unnamed_4
     6: 0000000000000000     0 SECTION LOCAL  DEFAULT    2
     7: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND PrintOutI
     8: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND PrintOutLn
     9: 0000000000000050    44 FUNC    GLOBAL DEFAULT    2 fibonacci
    10: 0000000000000000    69 FUNC    GLOBAL DEFAULT    2 main
     
```

### output LLVM IR code

```
./bin/fibol helloworld.fbl
```

```
@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [32 x i8] c"Hello World For Fibol Language!\00", align 1
@2 = private unnamed_addr constant [18 x i8] c"magic number is: \00", align 1
@3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

define void @main() {
start:
  %calltmp = call i64 @PrintOutLn(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @0, i32 0, i32 0))
  %calltmp1 = call i64 @PrintOutLn(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @1, i32 0, i32 0))
  %calltmp2 = call i64 @PrintOutLn(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @2, i32 0, i32 0))
  %calltmp3 = call i64 @fibonacci(i64 100)
  %calltmp4 = call i64 @PrintOutI(i64 %calltmp3)
  %calltmp5 = call i64 @PrintOutLn(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @3, i32 0, i32 0))
  ret void
}

declare i64 @New(i64)

declare i64 @Delete(i64)

declare i64 @PrintOutLn(i64)

declare i64 @PrintOut(i64)

declare i64 @PrintOutC(i64)

declare i64 @PrintOutI(i64)

declare i64 @Fibol()

define i64 @fibonacci(i64 %n) {
entry:
  %n1 = alloca i64, align 8
  store i64 %n, i64* %n1, align 4
  %greater = icmp ugt i64 %n, 1
  br i1 %greater, label %then, label %else

then:                                             ; preds = %entry
  %subtmp = sub i64 %n, 1
  %calltmp = call i64 @fibonacci(i64 %subtmp)
  %addtmp = add i64 %n, %calltmp
  ret i64 %addtmp

else:                                             ; preds = %entry
  ret i64 1

ifcont:                                           ; No predecessors!
  ret i64 0
}

```  

