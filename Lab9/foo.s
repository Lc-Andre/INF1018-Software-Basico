.text
.globl foo
foo:

pushq %rbp              /*base do RA da chamadora*/
movq %rsp, %rbp         /*base do RA da chamada*/
subq $16, %rsp          /*espaco no RA da chamada*/
movq  %rbx, -8(%rbp)
movq  %r12, -16(%rbp)

/*
Dicionário
Reg     Var
ebx     i
eax     s
*/

movl $0, %ebx
movl $0, %eax

movq %rdi, %r12

inicio_for:
cmpl %esi, %ebx
jge fim_for

/*calculando end*/
movl %ebx, %ecx /* copiando i para ecx*/
imul $4, %ecx
movslq %ecx, %rcx
addq %r12, %rcx

addl (%rcx), %eax /*s += a[i]*/

cmpl $0, (%rcx) /*if( a[i] == 0)*/
jne continua

/* a[i] = s */
movl %eax, (%rcx)

/* s = 0 */
movl $0, %eax

continua:
addl $1, %ebx
jmp inicio_for


fim_for:

movq -8(%rbp), %rbx
movq -16(%rbp), %r12

leave
ret
