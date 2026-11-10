/*
double foo2 (float a, float b) {
  return sin(a) + cos(b);
}
*/

.text
.globl foo2
foo2:

pushq %rbp
movq %rsp, %rbp
subq $16, %rsp

/*Dicionário
a xmm0
b xmm1
*/

/*salvando caller-saved*/

movss %xmm0, -4(%rbp)
movss %xmm1, -8(%rbp)

cvtss2sd %xmm0, %xmm0 /*double a*/
call sin

movsd %xmm0, -16(%rbp) /*resultado esta aqui*/

/*restaurando caller-saved*/
movss -4(%rbp), %xmm0
movss -8(%rbp), %xmm1 


cvtss2sd %xmm1, %xmm0 /*double b*/
call cos

addsd -16(%rbp), %xmm0

leave
ret




