#include <stdio.h>
#include <ctype.h>

int is_little(void){

    unsigned int a = 0x01020304;

    


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