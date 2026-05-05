#include <stdio.h>

int maior_que(int x, int y) {
    return x > y;
}

int boo(int *v, int n, int ref);

int main() {
    int v[] = {1, 5, 3, 7, 2};
    
    int resultado = boo(v, 5, 3);

    printf("%d\n", resultado);

    return 0;
}