
.text
.globl boo
boo:
pushq %rbp
movq %rsp, %rbp

subq $48, %rsp

movq %rbx, -8(%rbp) /*salvando callee-saved*/
movq %r12, -16(%rbp)
movq %r13, -24(%rbp)
movq %r14, -32(%rbp)
movq %r15, -40(%rbp)

/*Dicionario
lim edi
v rsi
n edx
i ebx
soma r15d
*/

/*inicializacao*/
movl %edi, %r12d
movq %rsi, %r13
movl %edx, %r14d

movl $0, %ebx /* i = 0*/
movl $0, %r15d /*soma = 0*/

inicio_for:

cmpl %r14d, %ebx /* i < n*/
jge sai_fora

/*calculo endereco de &v[i]*/
movslq %ebx, %rax /*extensao de i para 64 bits*/
imulq $4, %rax /*resultado e um endereco*/
addq %r13, %rax /* rax = &v[i])*/

movl (%rax), %edi
movl %r12d, %esi
call valida

/*comeca o if*/
cmpl $0, %eax 
je sai_do_if

/*calculo de endereco*/
movslq %ebx, %rax
imulq $4, %rax
addq %r13, %rax /* rax = &v[i])*/

addl (%rax), %r15d /* soma += v[i]*/


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