#include <stdio.h>
#include <math.h>

float foo1(float a, float b);

int main(void) {

    printf("%.2f\n", foo1(0.0f, 5.0f));
    printf("%.2f\n", foo1((float)M_PI_2, 5.0f));
    printf("%.2f\n", foo1((float)M_PI, 5.0f));

    return 0;
}