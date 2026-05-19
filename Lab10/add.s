/*
struct X {
  int val;
  struct X *next;
};

int add (struct X *x) {
  int a = 0;
  for (; x != NULL; x = x->next)
    a += x->val;
  return a;
}
*/

.text
.globl add
add:

pushq %rbp
movq %rsp, %rbp
subq  /*preciso ver depois quanto vou usar de memoria para este programa*/
movq %rbx, -8(%rbp)

/*
Dicionario
Reg   Var
rdi    x
ecx    a 

*/

while:
cmpq $0, %rdi
je fim_while


