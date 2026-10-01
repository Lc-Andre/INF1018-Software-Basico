.text
.globl boo
boo:

pushq %rbp
movq %rsp, %rbp
subq $32, %rsp
movq %r12, -8(%rbp) /*salvando callee-saved*/
movq %r13, -16(%rbp)
movq %r14, -24(%rbp)
/*Dicionario
px rdi
n esi
val edx

*/

movq %rdi, %r12
movl %esi, %r13d
movl %edx, %r14d

inicio_while:

cmpl $0, %r13d
je fim_while

movl 0(%r12), %edi
movl %r14d, %esi
call f

movl %eax, 4(%r12) /* px->val2 = f(px->val1,val)*/

addq $8, %r12
decl %r13d
jmp inicio_while

fim_while:

movq -8(%rbp), %r12 /*restaurando callee-saved*/
movq -16(%rbp), %r13
movq -24(%rbp), %r14 
leave
ret 