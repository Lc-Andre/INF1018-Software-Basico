#include <stdio.h>

void foo(int a[], int n);

int main() {
    int v[] = {1, 2, 0, 3};
    
    foo(v, 4);

    for (int i = 0; i < 4; i++) {
        printf("%d ", v[i]);
    }

    return 0;
}