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
subq $16, %rsp

/*
Dicionario
Reg   Var
rdi    x
ecx    a 
*/

movl $0, %ecx /* a = 0*/

while:
cmpq $0, %rdi
je fim_while

addl 0(%rdi), %ecx /* a += x->val*/
movq 8(%rdi), %rdi
jmp while

fim_while:
movl %ecx, %eax
leave
ret




