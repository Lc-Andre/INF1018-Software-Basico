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
movq %rsp, %rbp

/*
Dicionario
Reg   Var
rdi    x
ecx    a 
*/


