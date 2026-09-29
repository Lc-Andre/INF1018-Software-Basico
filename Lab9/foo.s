/*
void foo (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++) {
    s += a[i];
    if (a[i] == 0) {
      a[i] = s;
      s = 0;
    }
  }
}*/

.text
.globl foo
foo:

/*Dcionario
a rdi
n esi
i ecx
s r8d
*/

pushq %rbp
movq %rsp, %rbp

movl $0, %ecx /* i = 0 */
movl $0, %r8d /* s = 0 */

inicio_for:

cmpl %esi, %ecx /* i < n*/
jge fim_for

/*calculo de endereco*/

movslq %ecx, %rdx
imulq $4, %rdx
addq %rdi, %rdx /*rdx = &a[i]*/

addl (%rdx),%r8d /* s+= a[i]*/

cmpl $0, (%rdx)
jne fora_if

movl %r8d, (%rdx) /*a[i] = s*/
movl $0, %r8d /*s = 0*/

fora_if:
incl %ecx
jmp inicio_for

fim_for:
leave
ret

