#include <stdio.h>

struct S {
    double d;
    int i;
};

// declaração da função foo, usada por bar (implementação de teste)
float foo(int i, double d) {
    printf("  foo chamada com i=%d, d=%.2f\n", i, d);
    return (float)(i + d);
}

// bar está implementada em assembly (bar.s)
float bar(double ent, float *sai, int n);

int main() {
    float sai[5] = {1.5f, -2.0f, 3.0f, -0.5f, 4.0f};
    double ent = 10.0;
    int n = 5;

    printf("Antes:\n");
    for (int k = 0; k < n; k++)
        printf("  sai[%d] = %.2f\n", k, sai[k]);

    float resultado = bar(ent, sai, n);

    printf("Depois:\n");
    for (int k = 0; k < n; k++)
        printf("  sai[%d] = %.2f\n", k, sai[k]);

    printf("Resultado de bar = %.2f\n", resultado);

    return 0;
}