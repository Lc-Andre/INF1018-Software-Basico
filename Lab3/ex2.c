#include <stdio.h>

/*0x01010101 -> 0000 0001 0000 0001 0000 0001 0000 0001*/
/*0x01030101 -> 0000 0001 0000 0011 0000 0001 0000 0001*/
int odd_ones(unsigned int x)
{

  unsigned char *p = (unsigned char *)&x;
  int total = 0;

  for (int i = 0; i < sizeof x; i++)
  {
    unsigned char byte = *(p + i);

    for (int j = 0; j < 8; j++)
    {
      if (byte & (1 << j))
      {
        total++;
      }
    }
  }

  return total & 1;
}

int main()
{
  printf("%x tem numero %s de bits\n", 0x01010101, odd_ones(0x01010101) ? "impar" : "par");
  printf("%x tem numero %s de bits\n", 0x01030101, odd_ones(0x01030101) ? "impar" : "par");
  return 0;
}