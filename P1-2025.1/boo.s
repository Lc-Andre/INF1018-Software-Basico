.text
.globl boo
boo:

pushq %rbp
movq %rsp, %rbp
subq $32, %rsp

movq %rbx, -8(%rbp)


/*
Dicionario
Reg   Var
rdi     v
esi     n
edx     ref
ecx     i
r8d     cont
rbx     paux
*/

movl $0, %ecx
movl $0, %r8d


WHILE:
cmpl %esi, %ecx
jge FORA_WHILE

/*calculando endereco*/
movl %ecx, %ebx
imulq $4, %rbx
addq %rdi, %rbx

/*salvando caller-saved*/
movq    %rdi, -16(%rbp)
movl    %esi, -20(%rbp)
movl    %edx, -24(%rbp)
movl    %ecx, -28(%rbp)


movl (%rbx), %edi
movl  %edx, %esi
call maior_que

/*restaurando caller-saved salvos*/
movq    -16(%rbp), %rdi
movl    -20(%rbp), %esi
movl    -24(%rbp), %edx
movl    -28(%rbp), %ecx

cmpl $0, %eax
je CONTINUA

incl %r8d

CONTINUA:
incl %ecx   /* i++ */
jmp WHILE


FORA_WHILE:

movl %r8d, %eax   /* return cont */
movq -8(%rbp), %rbx

leave
ret