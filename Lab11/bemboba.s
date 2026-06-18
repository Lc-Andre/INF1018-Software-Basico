.text
.globl bemboba
bemboba:

    pushq %rbp
    movq  %rsp, %rbp
    subq  $32, %rsp

    # salva rbx
    movq  %rbx, -24(%rbp)

    /*
    Dicionário

    edi -> num
    ecx -> i
    rbx -> a

    local[0] -> -16(%rbp)
    local[1] -> -12(%rbp)
    local[2] -> -8(%rbp)
    local[3] -> -4(%rbp)
    */

    # i = 0
    movl $0, %ecx

    # a = local
    leaq -16(%rbp), %rbx

WHILE:

    cmpl $4, %ecx
    jge FIM_WHILE

    # *a = num
    movl %edi, (%rbx)

    # a++
    addq $4, %rbx

    # i++
    incl %ecx

    jmp WHILE

FIM_WHILE:

    # addl(local,4)

    leaq -16(%rbp), %rdi
    movl $4, %esi

    call addl

    # restaura rbx
    movq -24(%rbp), %rbx

    leave
    ret