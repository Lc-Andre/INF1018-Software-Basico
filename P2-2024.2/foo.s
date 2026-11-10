/*
double passa_alta(short *v, int limite);
int foo(short *v, int n, long filtro) {
int i;
float soma = 0;
for(i=0; i<n; i++) {
if(v[i] > filtro) {
soma += passa_alta(v[i], 3);
}
}
}

*/

.text
.globl foo
foo:

pushq %rbp
subq $48, %rsp
movq %rsp, %rbp

movq %rbx, -8(%rbp)
movq %r12, -16(%rbp)
movq %r13, -24(%rbp)
movq %r14, -32(%rbp)

/*Dicionario
i   ebx
v   r12
n   r13d
filtro  r14
*/

movq %rdi, %r12
movl %esi, %r13d
movq %rdx, %r14

movl $0, %ebx /*i = 0*/
movl $0, -48(%rbp) /*soma = 0*/

inicio_for:
cmpl %r13d, %ebx /* i < n*/
jge sai_fora

/*calc v[i]*/
movslq %ebx, %rax     /* i (32 bits) -> 64 bits */
imulq  $2, %rax       /* i * 2 */
addq   %r12, %rax     /* rax = &v[i]*/

movswq (%rax), %rcx
cmpq %r14, %rcx
jle sai_fora_if

movswq (%rax), %rdi
movl $3, %esi
call passa_alta

cvtss2sd -48(%rbp), %xmm1   /* soma (float) -> double */
addsd    %xmm0, %xmm1       /* soma + passa_alta(...) */
cvtsd2ss %xmm1, %xmm1       /* resultado -> float */
movss    %xmm1, -48(%rbp)   /* soma = ... */

sai_fora_if:
incl %ebx
jmp inicio_for

sai_fora:

movq -8(%rbp), %rbx 
movq -16(%rbp), %r12 
movq -24(%rbp), %r13 
movq -32(%rbp), %r14
leave
ret

