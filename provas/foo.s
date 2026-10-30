.text
.globl foo
foo:

pushq %rbp
movq %rsp, %rbp
subq $48, %rsp
movq %rbx, -8(%rbp)      /* salvando callee-saved */
movq %r12, -16(%rbp)
movq %r13, -24(%rbp)
movq %r14, -32(%rbp)
movq %r15, -40(%rbp)

/*
Dicionario
n  edi
c sil
v rdx
i ebx
s r15d
*/

movl %edi,%r12d
movb %sil,%r13b
movq %rdx, %r14
movl $0, %ebx /* i = 0*/
movl $0, %r15d /* s = 0*/

inicio_for:

cmpl %r12d, %ebx /* i < n*/
jge sai_fora

/*calculando endereco*/
movslq %ebx, %rax /* estende i para 64 bits*/
addq %r14, %rax /*&v[i]*/

cmpb %r13b, (%rax) /* v[i] >= c*/
jl sai_do_if

movb (%rax), %dil
movb %r13b, %sil
call gera
addl %eax, %r15d /*s += gera(v[i],c)*/

sai_do_if:
incl %ebx/* i++*/
jmp inicio_for

sai_fora:
movl %r15d, %eax
movq -8(%rbp), %rbx/* restaurando callee-saved */
movq -16(%rbp), %r12
movq -24(%rbp), %r13
movq -32(%rbp), %r14 
movq -40(%rbp),%r15
leave
ret 