#include<stdlib.h>

long long New(long long size){
    return (long long)malloc(size);
}

long long Delete(long long mem){
    free((void *)mem);
    return 0;
}