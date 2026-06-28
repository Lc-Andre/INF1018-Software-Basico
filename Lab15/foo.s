.text
.globl foo
foo:

pushq %rbp
movq %rsp, %rbp
subq $16, %rsp

/*
Dicionario
Reg     Var
xmm0    a
xmm1    b
xmm2    a copia
xmm3    b copia
*/
cvtss2sd %xmm1, %xmm1 /*(double) b*/
movsd %xmm0, %xmm2 
movsd %xmm1, %xmm3


addsd %xmm0, %xmm3 /* xmm3 = a + b*/
subsd %xmm1, %xmm2 /* a - b*/

mulsd %xmm2, %xmm3
cvtsd2ss %xmm3, %xmm3
movss %xmm3, %xmm0
leave
ret


