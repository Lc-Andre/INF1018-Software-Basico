#include <stdio.h>

void b(short z, int y, long x) {
    printf("Values: %hd, %d, %ld\n", z, y, x);
}

void a(long x, int y, short z) {
    b(z, y, x);
}

int main(void) {
    a(1,2,3);
    return 0;
}