#include <stdio.h>

unsigned char switch_byte(unsigned char x)
{
    unsigned char a = x >> 4;
    unsigned char b = x << 4;

    return a | b;
}
int main(void)
{

    unsigned char n = 0xAB;

    printf("%02x\n", n);
    printf("Trocando...\n");
    printf("%02x\n", switch_byte(n));

    return 0;
}