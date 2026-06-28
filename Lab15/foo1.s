.text
.globl foo1
foo1:

pushq %rbp
movq %rsp,%rbp
subq $16,%rsp

# salva b
movss %xmm1,-4(%rbp)

# float -> double
cvtss2sd %xmm0,%xmm0

call sin

# double -> float
cvtsd2ss %xmm0,%xmm0

# recupera b
movss -4(%rbp),%xmm1

# soma
addss %xmm1,%xmm0

leave
ret