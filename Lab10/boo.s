/*
struct X {
  int val1;
  int val2;
};

int f(int i, int v);

void boo (struct X *px, int n, int val) {
  while (n--) {
    px->val2 = f(px->val1, val);
    px++;
  }
}
*/

.text
.globl boo
boo:

pushq %rbp
movq %rsp, %rbp
subq $16, %rsp

/*
Dicionario
Reg     Var
rdi     px
esi     n
edx     val
*/

WHILE:
cmpl $0, %esi
je FIM_WHILE


/*Salvar registradores caller saved necessarios*/

movq %rdi, -8(%rbp)
movl %esi, -12(%rbp)
movl %edx, -16(%rbp)

movl 0(%rdi), %edi
movl %edx, %esi

call f

movq -8(%rbp), %rdi
movl -12(%rbp), %esi
movl -16(%rbp), %edx 


movl %eax, 4(%rdi)

addq $8, %rdi

subl $1, %esi /* n--*/

jmp WHILE

FIM_WHILE:

leave
ret









