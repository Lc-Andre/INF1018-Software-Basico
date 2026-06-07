#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cria_func.h"

/* ============================================================
 * Funções auxiliares usadas nos testes
 * ============================================================ */

int mult(int x, int y) {
    return x * y;
}

int soma(int x, int y) {
    return x + y;
}

int soma3(int x, int y, int z) {
    return x + y + z;
}

int identidade(int x) {
    return x;
}

/* Retorna 1 se s1 e s2 têm o mesmo prefixo de n bytes, 0 caso contrário */
int mesmo_prefixo_raw(const void *s1, const void *s2, int n) {
    return memcmp(s1, s2, (size_t)n) == 0 ? 1 : 0;
}

/* ============================================================
 * Contadores de teste
 * ============================================================ */
static int tests_run    = 0;
static int tests_passed = 0;

#define CHECK(cond, msg)                                              \
    do {                                                              \
        tests_run++;                                                  \
        if (cond) {                                                   \
            tests_passed++;                                           \
            printf("  [OK] %s\n", msg);                              \
        } else {                                                      \
            printf("  [FAIL] %s\n", msg);                            \
        }                                                             \
    } while(0)

/* ============================================================
 * TESTE 1 — Um parâmetro PARAM (identidade)
 * Função gerada: g(x) = identidade(x)
 * ============================================================ */
void teste_1param_param(void) {
    printf("\n[Teste 1] 1 param PARAM — identidade\n");

    unsigned char codigo[500];
    DescParam params[1];

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;

    cria_func(identidade, params, 1, codigo);

    typedef int (*fp)(int);
    fp g = (fp)codigo;

    CHECK(g(0)   == 0,   "g(0) == 0");
    CHECK(g(42)  == 42,  "g(42) == 42");
    CHECK(g(-7)  == -7,  "g(-7) == -7");
}

/* ============================================================
 * TESTE 2 — Um parâmetro FIX (constante)
 * Função gerada: g() = identidade(99)
 * ============================================================ */
void teste_1param_fix(void) {
    printf("\n[Teste 2] 1 param FIX — identidade(99)\n");

    unsigned char codigo[500];
    DescParam params[1];

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = FIX;
    params[0].valor.v_int = 99;

    cria_func(identidade, params, 1, codigo);

    typedef int (*fp)(void);
    fp g = (fp)codigo;

    CHECK(g() == 99, "g() == 99");
    CHECK(g() == 99, "g() == 99 (repetida)");
}

/* ============================================================
 * TESTE 3 — Um parâmetro IND
 * Função gerada: g() = identidade(*ptr)
 * ============================================================ */
void teste_1param_ind(void) {
    printf("\n[Teste 3] 1 param IND — identidade(*ptr)\n");

    unsigned char codigo[500];
    DescParam params[1];
    int var = 7;

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = IND;
    params[0].valor.v_ptr = &var;

    cria_func(identidade, params, 1, codigo);

    typedef int (*fp)(void);
    fp g = (fp)codigo;

    CHECK(g() == 7, "var=7: g() == 7");
    var = 42;
    CHECK(g() == 42, "var=42: g() == 42");
    var = -3;
    CHECK(g() == -3, "var=-3: g() == -3");
}

/* ============================================================
 * TESTE 4 — Exemplo do enunciado: mult com segundo param FIX=10
 * Função gerada: g(x) = mult(x, 10)
 * ============================================================ */
void teste_mult_fix(void) {
    printf("\n[Teste 4] mult(x, 10) — segundo param FIX\n");

    unsigned char codigo[500];
    DescParam params[2];

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;

    params[1].tipo_val = INT_PAR;
    params[1].orig_val = FIX;
    params[1].valor.v_int = 10;

    cria_func(mult, params, 2, codigo);

    typedef int (*fp)(int);
    fp g = (fp)codigo;

    CHECK(g(1)  == 10,  "g(1) == 10");
    CHECK(g(5)  == 50,  "g(5) == 50");
    CHECK(g(10) == 100, "g(10) == 100");
    CHECK(g(-3) == -30, "g(-3) == -30");
}

/* ============================================================
 * TESTE 5 — Exemplo do enunciado: mult com primeiro param IND
 * Função gerada: g() = mult(*ptr, 10)
 * ============================================================ */
void teste_mult_ind_fix(void) {
    printf("\n[Teste 5] mult(*i, 10) — primeiro IND, segundo FIX\n");

    unsigned char codigo[500];
    DescParam params[2];
    int i;

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = IND;
    params[0].valor.v_ptr = &i;

    params[1].tipo_val = INT_PAR;
    params[1].orig_val = FIX;
    params[1].valor.v_int = 10;

    cria_func(mult, params, 2, codigo);

    typedef int (*fp)(void);
    fp g = (fp)codigo;

    int ok = 1;
    for (i = 1; i <= 10; i++) {
        if (g() != i * 10) { ok = 0; break; }
    }
    CHECK(ok, "dezenas de 1 a 10 corretas");
}

/* ============================================================
 * TESTE 6 — 2 params PARAM
 * Função gerada: g(x, y) = soma(x, y)
 * ============================================================ */
void teste_2params_param(void) {
    printf("\n[Teste 6] soma(x, y) — ambos PARAM\n");

    unsigned char codigo[500];
    DescParam params[2];

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;

    params[1].tipo_val = INT_PAR;
    params[1].orig_val = PARAM;

    cria_func(soma, params, 2, codigo);

    typedef int (*fp)(int, int);
    fp g = (fp)codigo;

    CHECK(g(3, 4)   == 7,   "g(3,4) == 7");
    CHECK(g(0, 0)   == 0,   "g(0,0) == 0");
    CHECK(g(-5, 5)  == 0,   "g(-5,5) == 0");
    CHECK(g(100, 1) == 101, "g(100,1) == 101");
}

/* ============================================================
 * TESTE 7 — params invertidos: FIX primeiro, PARAM segundo
 * Função gerada: g(y) = soma(100, y)
 * ============================================================ */
void teste_fix_depois_param(void) {
    printf("\n[Teste 7] soma(100, y) — primeiro FIX, segundo PARAM\n");

    unsigned char codigo[500];
    DescParam params[2];

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = FIX;
    params[0].valor.v_int = 100;

    params[1].tipo_val = INT_PAR;
    params[1].orig_val = PARAM;

    cria_func(soma, params, 2, codigo);

    typedef int (*fp)(int);
    fp g = (fp)codigo;

    CHECK(g(0)   == 100, "g(0) == 100");
    CHECK(g(5)   == 105, "g(5) == 105");
    CHECK(g(-10) == 90,  "g(-10) == 90");
}

/* ============================================================
 * TESTE 8 — 3 params: PARAM, FIX, PARAM
 * Função gerada: g(x, z) = soma3(x, 50, z)
 * ============================================================ */
void teste_3params_param_fix_param(void) {
    printf("\n[Teste 8] soma3(x, 50, z) — PARAM, FIX, PARAM\n");

    unsigned char codigo[500];
    DescParam params[3];

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;

    params[1].tipo_val = INT_PAR;
    params[1].orig_val = FIX;
    params[1].valor.v_int = 50;

    params[2].tipo_val = INT_PAR;
    params[2].orig_val = PARAM;

    cria_func(soma3, params, 3, codigo);

    typedef int (*fp)(int, int);
    fp g = (fp)codigo;

    CHECK(g(0, 0)   == 50,  "g(0,0) == 50");
    CHECK(g(10, 5)  == 65,  "g(10,5) == 65");
    CHECK(g(-10, 0) == 40,  "g(-10,0) == 40");
}

/* ============================================================
 * TESTE 9 — 3 params: FIX, FIX, FIX
 * Função gerada: g() = soma3(1, 2, 3)
 * ============================================================ */
void teste_3params_todos_fix(void) {
    printf("\n[Teste 9] soma3(1, 2, 3) — todos FIX\n");

    unsigned char codigo[500];
    DescParam params[3];

    params[0].tipo_val = INT_PAR; params[0].orig_val = FIX; params[0].valor.v_int = 1;
    params[1].tipo_val = INT_PAR; params[1].orig_val = FIX; params[1].valor.v_int = 2;
    params[2].tipo_val = INT_PAR; params[2].orig_val = FIX; params[2].valor.v_int = 3;

    cria_func(soma3, params, 3, codigo);

    typedef int (*fp)(void);
    fp g = (fp)codigo;

    CHECK(g() == 6, "g() == 6");
}

/* ============================================================
 * TESTE 10 — PTR_PAR com memcmp (exemplo do enunciado)
 * ============================================================ */
void teste_memcmp_ptr(void) {
    printf("\n[Teste 10] memcmp com PTR_PAR FIX e PARAMs\n");

    unsigned char codigo[500];
    DescParam params[3];

    char fixa[] = "quero saber se a outra string e um prefixo dessa";

    params[0].tipo_val = PTR_PAR;
    params[0].orig_val = FIX;
    params[0].valor.v_ptr = fixa;

    params[1].tipo_val = PTR_PAR;
    params[1].orig_val = PARAM;

    params[2].tipo_val = INT_PAR;
    params[2].orig_val = PARAM;

    cria_func(memcmp, params, 3, codigo);

    typedef int (*fp)(void*, size_t);
    fp mesmo_prefixo = (fp)codigo;

    char s[] = "quero saber tudo";

    CHECK(mesmo_prefixo(s, 12) == 0, "prefixo-12 igual (retorna 0)");
    CHECK(mesmo_prefixo(s, strlen(s)) != 0, "prefixo-strlen(s) diferente (retorna != 0)");
}

/* ============================================================
 * TESTE 11 — PTR_PAR IND: ponteiro lido de variável
 * Função gerada: g(n) = memcmp(*pptr, s, n)
 * ============================================================ */
void teste_ptr_ind(void) {
    printf("\n[Teste 11] PTR_PAR IND — ponteiro lido de variável\n");

    unsigned char codigo[500];
    DescParam params[3];

    char base[] = "hello world";
    char *ptr_base = base; /* variável cujo valor é um ponteiro */

    params[0].tipo_val = PTR_PAR;
    params[0].orig_val = IND;
    params[0].valor.v_ptr = &ptr_base; /* endereço da variável ptr_base */

    params[1].tipo_val = PTR_PAR;
    params[1].orig_val = PARAM;

    params[2].tipo_val = INT_PAR;
    params[2].orig_val = PARAM;

    cria_func(memcmp, params, 3, codigo);

    typedef int (*fp)(void*, size_t);
    fp g = (fp)codigo;

    char s1[] = "hello world";
    char s2[] = "hello kitty";

    CHECK(g(s1, 11) == 0, "s1 == base (11 bytes)");
    CHECK(g(s2, 5)  == 0, "s2 == base (5 bytes: 'hello')");
    CHECK(g(s2, 11) != 0, "s2 != base (11 bytes)");
}

/* ============================================================
 * TESTE 12 — 3 params todos PARAM
 * Função gerada: g(x,y,z) = soma3(x,y,z)
 * ============================================================ */
void teste_3params_todos_param(void) {
    printf("\n[Teste 12] soma3(x,y,z) — todos PARAM\n");

    unsigned char codigo[500];
    DescParam params[3];

    params[0].tipo_val = INT_PAR; params[0].orig_val = PARAM;
    params[1].tipo_val = INT_PAR; params[1].orig_val = PARAM;
    params[2].tipo_val = INT_PAR; params[2].orig_val = PARAM;

    cria_func(soma3, params, 3, codigo);

    typedef int (*fp)(int, int, int);
    fp g = (fp)codigo;

    CHECK(g(1, 2, 3)    == 6,   "g(1,2,3) == 6");
    CHECK(g(10, 20, 30) == 60,  "g(10,20,30) == 60");
    CHECK(g(-1, -1, -1) == -3,  "g(-1,-1,-1) == -3");
}

/* ============================================================
 * main
 * ============================================================ */
int main(void) {
    printf("=== Testes cria_func ===\n");

    teste_1param_param();
    teste_1param_fix();
    teste_1param_ind();
    teste_mult_fix();
    teste_mult_ind_fix();
    teste_2params_param();
    teste_fix_depois_param();
    teste_3params_param_fix_param();
    teste_3params_todos_fix();
    teste_memcmp_ptr();
    teste_ptr_ind();
    teste_3params_todos_param();

    printf("\n=== Resultado: %d/%d testes passaram ===\n", tests_passed, tests_run);
    return (tests_passed == tests_run) ? 0 : 1;
}