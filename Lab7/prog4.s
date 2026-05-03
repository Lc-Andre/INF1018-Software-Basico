


.data 
nums: .int 65, -105, 111, 34
Sf: .string "soma = %d\n"

.text
.globl main
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)
/********************************************************/


movl $0, %ebx /* i = 0*/
movl $0, %eax /* s = 0*/
movq $nums, %r12


inicio_for:

cmpl $4, %ebx /* i < 4*/
jge fim_for


/*calculo do endereco*/

movl %ebx, %ecx
imul $4, %ecx 
movslq %ecx, %rcx
addq %r12, %rcx /*calculo do endereco termina aqui*/



addl (%rcx), %eax /* s += nums[i]*/

addl $1, %ebx 
jmp inicio_for

fim_for:
/********************************************************/
/* printf                                               */
  movq $Sf, %rdi
  movl %eax, %esi
  call printf
/********************************************************/

/********************************************************/
/* epilogo - NAO MEXER                                  */
  movq $0, %rax
  movq -8(%rbp), %rbx
  movq -16(%rbp), %r12
  leave
  ret
/********************************************************/

