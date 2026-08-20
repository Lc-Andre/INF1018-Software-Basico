#include <stdio.h>

/*0x01010101 -> 0000 0001 0000 0001 0000 0001 0000 0001*/
/*0x01030101 -> 0000 0001 0000 0011 0000 0001 0000 0001*/
int odd_ones(unsigned int x)
{

  int total = 0;

  for(int i = 0; i < 32; i++){


    if( x & 1){
      total++;
    }
    
    x = x >> 1;

  }

  return total & 1;
}

int main()
{
  printf("%x tem numero %s de bits\n", 0x01010101, odd_ones(0x01010101) ? "impar" : "par");
  printf("%x tem numero %s de bits\n", 0x01030101, odd_ones(0x01030101) ? "impar" : "par");
  return 0;
}