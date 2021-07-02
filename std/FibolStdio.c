
#include<stdio.h>
#include"include/DataType.h"

NUMBER_INT PrintOutLn(NUMBER_INT s){
    puts((char *)s);
    return 0;
} 

NUMBER_INT PrintOut(NUMBER_INT s){
    printf("%s",(char *)s);
    return 0;
} 

NUMBER_INT PrintOutI(NUMBER_INT i){
    printf("%lld\n",i);
    return 0;
}

NUMBER_INT PrintOutC(NUMBER_INT c){
    printf("%c\n",(char)c);
    return 0;
}

NUMBER_INT InputI(){
    NUMBER_INT i = 0;
    scanf("%lld",&i);
    return i;
}

NUMBER_INT InputC(){
    NUMBER_INT i = 0;
    scanf("%lld",&i);
    return i;
}

NUMBER_INT Fibol(){
    puts("Fibol:");
    puts("  Fibol is a easy language build with Flex, Bison, LLVM");
    puts("Enjoy ;)");
    return 0;
}