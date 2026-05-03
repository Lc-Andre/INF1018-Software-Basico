  /*
int filtro(int x, int lim) {
  if (x < lim)
    return 0;
  else
    return x;
}

#include <stdio.h>
#define LIM 1

int filtro(int x, int lim);

int nums[5] = {3,-5,7,8,-2};

int main() {
  int i;
  int *p = nums;
  for (i = 0; i < 5; i++) {
    printf("%d\n", filtro(*p,LIM));
    p++;
  }
  return 0;
}
*/
  
  
  .data
  nums: .int 3, -5, 7, 8, -2
  s1:   .string "%d\n"

  .text
  .globl main
  main:
  /* prologo */
     pushq %rbp
     movq  %rsp, %rbp
     subq  $16, %rsp
     movq  %rbx, -8(%rbp)
     movq  %r12, -16(%rbp)

  /* coloque seu codigo aqui */
    movl $0, %ebx /*i = 0*/
    movq $nums, %r12 /* r12 = &nums*/

    
    inicio_for:

    cmpl $5, %ebx
    jge fim_for

    /*filtro*/
    
    movl (%r12), %edi
    movl $1, %esi
    call filtro


    /*printf*/
    movq $s1, %rdi
    movl %eax, %esi
    movl $0, %eax
    call printf

    addq $4, %r12
    addl $1, %ebx

    jmp inicio_for


fim_for:
  movl $0, %eax
  /* finalizacao */
     movq -8(%rbp), %rbx
     movq -16(%rbp), %r12
     leave
     ret