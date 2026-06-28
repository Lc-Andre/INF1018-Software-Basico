.text
.globl foo2
foo2:

pushq %rbp
movq  %rsp, %rbp
subq  $16, %rsp

/*
Dicionário
Reg     Var
xmm0    a / resultado
xmm1    b

RA
-8(%rbp)   b (float)
-16(%rbp)  resultado de sin(a) (double)
*/

# Salva b
movss %xmm1, -8(%rbp)

# -------- sin(a) --------

# float -> double
cvtss2sd %xmm0, %xmm0

call sin

# Guarda sin(a)
movsd %xmm0, -16(%rbp)

# -------- cos(b) --------

# Recupera b
movss -8(%rbp), %xmm0

# float -> double
cvtss2sd %xmm0, %xmm0

call cos

# -------- soma --------

# xmm0 = cos(b) + sin(a)
addsd -16(%rbp), %xmm0

leave
ret