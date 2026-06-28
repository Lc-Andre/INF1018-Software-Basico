#include <stdio.h>
#include <math.h>

double foo2(float a, float b);

int main(void)
{
    printf("%.2lf\n", foo2(0.0f, 0.0f));
    printf("%.2lf\n", foo2((float)M_PI_2, 0.0f));
    printf("%.2lf\n", foo2((float)M_PI, (float)M_PI));

    return 0;
}