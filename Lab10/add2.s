/*
struct X {
  int val;
  struct X *next;
};

int add2 (struct X *x) {
  if (x == NULL) return 0;
  else return x->val + add2(x->next);
}
*/

.text
.globl add2
add2:

    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp

    /* salva r12 (callee-saved) */
    movq  %r12, -8(%rbp)

    /*
    Dicionário
    Reg     Var
    rdi     x
    r12d    x->val
    */

    /* if (x == NULL) */
    cmpq $0, %rdi
    jne ELSE

    /* return 0 */
    movl $0, %eax
    jmp FIM

ELSE:
    /* r12d = x->val */
    movl 0(%rdi), %r12d

    /* parâmetro da chamada recursiva: x->next */
    movq 8(%rdi), %rdi

    call add2

    /* retorno += x->val */
    addl %r12d, %eax

FIM:
    /* restaura r12 */
    movq -8(%rbp), %r12

    leave
    ret