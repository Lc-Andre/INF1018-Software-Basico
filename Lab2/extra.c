#include <stdio.h>
#include <ctype.h>

int is_little(void){
    unsigned int n = 0x01020304;
    
    char *p = (char*) &n;
    
    if(*p){
        return 1;
    }else{
        return 0;
    }
}


int main(void){

    int res = is_little();
    
    if(res == 1){
        printf("Little endian\n");
    }else{
        printf("Big endian\n");
    }


    return 0;
}