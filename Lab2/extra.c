#include <stdio.h>
#include <ctype.h>

int is_little(void){

    unsigned int a = 0x01020304;
    unsigned char *p = (unsigned char*)&a;
    
    if(p[0] == (a & 0x000000FF)){
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