#include <stdio.h>


unsigned char rotate_left(unsigned char x, int n){

    unsigned char result = (x << n) | x >> (8 - n);
    return result;
}

int main(void){


unsigned char a = 0x61;
unsigned result = rotate_left(a, 7);


printf("0x%x\n", result);

// um "rotate left" de 1 bit resulta no valor 0xc2 (1100 0010)
// um "rotate left" de 2 bits resulta no valor 0x85 (1000 0101)
// um "rotate left" de 7 bits resulta no valor 0xb0 (1011 0000)

}