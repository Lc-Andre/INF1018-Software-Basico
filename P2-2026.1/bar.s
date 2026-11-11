.text
.globl bar
bar:

pushq %rbp
movq %rsp, %rbp
subq $160, %rsp
/*salvando callee-saved*/
movq %rbx, -8(%rbp)
movq %r12, -16(%rbp)
movq %r13, -24(%rbp)
movq %r14, -32(%rbp)
movq %r15, -48(%rbp)
movsd %xmm0, -64(%rbp)

/*Dicionario
ent     -64(%rbp)
sai     r13
n       r12d
i       ebx
p       r14
v       r15
r       -68(%rbp)
*/

movq %rdi, %r13
movl %esi, %r12d
movl $0, -68(%rbp) /*r = 0.0*/
movq %r13, %r14 /* p = sai */
movl $0, %ebx /*i = 0*/
leaq -152(%rbp), %r15 /* v = &v[0] */

inicio_for:
cmpl %r12d, %ebx /* i < n*/
jge sai_fora

/* if (*p < 0) *p = 0; */
movl $0, %eax
cvtsi2ss %eax, %xmm1 /* xmm1 = 0.0 */
movss (%r14), %xmm2 /* xmm2 = *p */
ucomiss %xmm1, %xmm2 /* compara *p com 0.0 */
jae sai_fora_if
movl $0, (%r14) /* *p = 0 */
sai_fora_if:

/*Calculo v[i]*/
movslq %ebx, %rax
imulq $16, %rax
addq %r15, %rax /* rax = &v[i]*/

movl 8(%rax), %edi /* v[i].i */
movsd -64(%rbp), %xmm0 /* ent */
call foo

addss -68(%rbp), %xmm0 /* r += foo(...) */
movss %xmm0, -68(%rbp)

addq $4, %r14 /* p++ */
incl %ebx /* i++ */
jmp inicio_for

sai_fora:

movss -68(%rbp), %xmm0 /* return r */

movq -8(%rbp),%rbx 
movq -16(%rbp), %r12
movq -24(%rbp), %r13 
movq -32(%rbp), %r14 
movq -48(%rbp), %r15 

leave
ret