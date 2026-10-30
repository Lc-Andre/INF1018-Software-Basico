/*
int passa_alta(short v, int limite);
int foo(short *v, int n, long filtro) {
int i, soma = 0;
for(i=0; i<n; i++) {
if(v[i] > filtro) {
soma += passa_alta(v[i], 3);
}
}
return soma;
}
*/
.text
.globl foo
foo:
pushq %rbp
movq %rsp, %rbp

subq $48, %rsp

movq %rbx, -8(%rbp) /*salvando callee-saved*/
movq %r12, -16(%rbp)
movq %r13, -24(%rbp)
movq %r14, -32(%rbp)
movq %r15, -40(%rbp)

/*Dicionario
v r12
n r13d
filtro r14
i ebx
soma r15d
*/

/*inicializacao*/
movq %rdi, %r12
movl %esi, %r13d
movq %rdx, %r14
movl $0, %ebx /* i = 0*/
movl $0, %r15d /*soma = 0*/

inicio_for:

cmpl %r13d, %ebx /* i < n*/
jge sai_fora

/*calculo endereco de &v[i]*/
movslq %ebx, %rax /*extensao de i para 64 bits*/
imulq $2, %rax /*resultado e um endereco*/
addq %r12, %rax /* rax = &v[i])*/

movswq (%rax), %rcx   
cmpq   %r14, %rcx 
jle sai_do_if

movw (%rax), %di
movl $3, %esi
call passa_alta

addl %eax, %r15d /* soma +=  passa_alta(v[i], 3)*/

sai_do_if:

incl %ebx /*i++*/
jmp inicio_for

sai_fora:
movl %r15d, %eax

movq -8(%rbp), %rbx  /*restaurando calle-saved*/
movq -16(%rbp), %r12
movq -24(%rbp), %r13 
movq -32(%rbp),%r14 
movq -40(%rbp),%r15

leave
ret