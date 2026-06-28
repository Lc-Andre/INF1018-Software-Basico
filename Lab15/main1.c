#include <stdio.h>

float foo(double a, float b);

int main(void) {
    printf("%.2f\n", foo(10.0, 2.0f));
    printf("%.2f\n", foo(5.0, 1.0f));   // 24.00
    printf("%.2f\n", foo(8.0, 3.0f));   // 55.00
    printf("%.2f\n", foo(2.0, 2.0f));   // 0.00
    printf("%.2f\n", foo(7.0, 0.0f));   // 49.00
    return 0;
}

