.extern dados

.data
Sf: .string "%d\n"

.text
.globl main
main:

pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq -16(%rbp), %r12
movq -8(%rbp), %rbx


movl $0, %eax /* max = 0 */
movl $0, %ecx /* i = 0 */
movq $dados, %r12 /* r12 = &dados */

inicio_for:
cmpl $3, %ecx
jge fim_for

cmpl $0, 0(%r12)
je depois_if

cmpl %eax, 4(%r12)
jle depois_if

movl 4(%r12), %eax

depois_if:

addq $8, %r12
addl $1, %ecx

jmp inicio_for

/* Funcao printf */
fim_for:
movq $Sf, %rdi
movl %eax, %esi
movl $0, %eax
call printf

/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq    -16(%rbp), %r12 /* recupera r12 */
  movq    -8(%rbp), %rbx  /* recupera rbx */
  leave
  ret      
/***************************************************************/

