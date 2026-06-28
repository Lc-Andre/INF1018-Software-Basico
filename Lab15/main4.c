#include <stdio.h>
#include <math.h>

double foo3(double *, int);

int main(void)
{
    double v[3]={0.0,M_PI_2,M_PI};

    printf("%.2lf\n",foo3(v,3));

    return 0;
}