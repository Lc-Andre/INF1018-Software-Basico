#include <stdio.h>

typedef struct x X;

struct x
{
  int a;
  short b;
  int c;
};

void dump(void *p, int n)
{
  unsigned char *p1 = p;
  while (n--)
  {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main(void)
{

  X var = {0xa1a2a3a4, 0xb1b2, 0xc1c2c3c4};

  printf("Tamanho da estrutura X: %ld\n", sizeof(var));
  printf("X: \n");
  dump(&var, sizeof(X));

  return 0;
}
