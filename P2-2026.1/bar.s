.text
.globl bar
bar:

pushq %rbp
movq %rsp, %rbp
subq $128, %rsp

movq %rbx, -8(%rbp)
movsd %xmm0, -120(%rbp)    /* salva ent, já que xmm0 vai ser reaproveitado */

/*Dicionario
Reg     Var
xmm0    ent
rdi     sai
esi     n
xmm1    r
r8      p
ecx     i
rbx     paux
*/

movl    $0, %eax
cvtsi2ss %eax, %xmm1 /* r = 0*/
movq  %rdi, %r8 /* p = sai*/
movl $0, %ecx /* i = 0*/
leaq    -88(%rbp), %rbx /*inicio do vetor*/

inicio_for:
cmpl %esi, %ecx
jge fim_for

movl    $0, %eax
cvtsi2ss %eax, %xmm0

movss (%r8), %xmm2 
ucomiss %xmm0, %xmm2 /* (*p < 0) */
jae depois_if
movss %xmm0, (%r8) /* *p = 0*/

depois_if:
/*Calculo de v[i]*/
movslq  %ecx, %rax         /* rax = i */
imul    $16, %rax            /* rax = i * 16  */
addq    %rbx, %rax          /* rax = &v[0] + i*16 = &v[i] */
movl    8(%rax), %edi       /* edi = v[i].i e tambem aproveitando para passar como arg de foo*/


/*salvar registradores caller-saved antes da chamada de foo*/
movss %xmm1, -96(%rbp)     /* salva r */
movq  %r8, -104(%rbp)      /* salva p */
movl  %ecx, -112(%rbp)     /* salva i */

movsd -120(%rbp), %xmm0    /* recarrega ent (2o argumento de foo) */
call  foo

/* r += foo(...) */
movss -96(%rbp), %xmm2     /* xmm2 = r antigo */
addss %xmm2, %xmm0         /* xmm0 = foo(...) + r */
movss %xmm0, %xmm1         /* r = xmm0 aqui seria o novo r*/

movq  -104(%rbp), %r8      /* restaura p */
movl  -112(%rbp), %ecx     /* restaura i */

addq $4, %r8                /* p++ */
addl $1, %ecx                /* i++ */
jmp inicio_for


fim_for:
movss %xmm1, %xmm0          /* return r (valor de retorno em xmm0) */

movq -8(%rbp), %rbx          /* restaura rbx */
leave
ret
