#include <stdio.h>
#include <sys/mman.h>

typedef int (*funcp) (int x);

int add(int x){
    return x + 1;
}

/* static + alinhado a página: precisa estar perto de add (alcance de 32 bits)
   e numa página própria para o mprotect */
static unsigned char codigo[] __attribute__((aligned(4096))) =
    {0x55,0x48,0x89,0xe5,0xe8,0x00,0x00,0x00,0x00,0xc9,0xc3};

int main(void){
    int i;
    funcp f = (funcp)codigo;

    /* deslocamento = endereço de add - endereço da instrução seguinte ao call */
    int desloc = (int)((long)add - (long)(codigo + 9));

    /* grava os 4 bytes em little-endian (menos significativo primeiro) */
    codigo[5] = desloc & 0xFF;
    codigo[6] = (desloc >> 8) & 0xFF;
    codigo[7] = (desloc >> 16) & 0xFF;
    codigo[8] = (desloc >> 24) & 0xFF;

    /* torna a página do array executável */
    mprotect(codigo, 4096, PROT_READ | PROT_WRITE | PROT_EXEC);

    i = (*f)(10);
    printf("%d\n", i);
    return 0;
}