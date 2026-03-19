#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %d\n", p1, *p1); //alteração nesta funcao para imprimir o valor ASCII
    p1++;
  }
}


int main (void) {
  //char i = 97;

  char j = 'A';
  char k = ' ';
  char w = '\n';
  char h = '$';





  //dump(&i, sizeof(i));
  printf("Codigo ASCII de A: ");
  dump(&j, sizeof(j));
  printf("Codigo ASCII de ' ': ");
  dump(&k, sizeof(k));
  printf("Codigo ASCII de \\n: ");
  dump(&w, sizeof(w));
  printf("Codigo ASCII de $: ");
  dump(&h, sizeof(h));


  return 0;
}