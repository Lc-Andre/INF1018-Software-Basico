.text
.globl foo3
foo3:

pushq %rbp
movq %rsp, %rbp
subq $32, %rsp

# salvar callee-saved usado
movq %rbx, -8(%rbp)

/*
Dicionário

Reg     Var

rdi     a
esi     n
ebx     i

RA

-8(%rbp)    rbx salvo
-16(%rbp)   r (double)
-24(%rbp)   rdi salvo
-28(%rbp)   esi salvo
*/

# i = 0
movl $0, %ebx

# r = 0.0
movq $0, -16(%rbp)

WHILE:

cmpl %esi, %ebx
jge FIM

# salva caller-saved
movq %rdi, -24(%rbp)
movl %esi, -28(%rbp)

# parâmetro de sin = *a
movsd (%rdi), %xmm0

call sin

# restaura caller-saved
movq -24(%rbp), %rdi
movl -28(%rbp), %esi

# r += sin(*a)
addsd -16(%rbp), %xmm0
movsd %xmm0, -16(%rbp)

# a++
addq $8, %rdi

# i++
incl %ebx

jmp WHILE

FIM:

movsd -16(%rbp), %xmm0

# restaura callee-saved
movq -8(%rbp), %rbx

leave
ret