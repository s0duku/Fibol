#include<stdlib.h>
#include"include/DataType.h"

NUMBER_INT New(NUMBER_INT size){
    return (long long)malloc(size);
}

NUMBER_INT Delete(NUMBER_INT mem){
    free((void *)mem);
    return 0;
}

