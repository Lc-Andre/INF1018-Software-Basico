


.text
.globl fat
fat:

pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %rbx, -8(%rbp)

cmpl $0, %edi
jne else_fat


movl $1, %eax
jmp fim_fat

else_fat:

movl %edi, %ebx
subl $1, %edi
call fat
imull %ebx, %eax

fim_fat:
movq -8(%rbp), %rbx
leave
ret 