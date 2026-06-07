#include "cria_func.h"
#include <stdint.h>
#include <string.h>

/*
 * Geração de código de máquina x86-64 (System V AMD64 ABI)
 *
 * Registradores de argumento inteiro/ponteiro (em ordem):
 *   arg0 -> rdi,  arg1 -> rsi,  arg2 -> rdx
 *
 * Estratégia geral do código gerado:
 *
 *   1. Prólogo: push rbp / mov rbp,rsp / (alinha pilha a 16 bytes via sub rsp,8 se necessário)
 *   2. Salvar registradores de entrada (PARAM) que serão sobrescritos:
 *      Os parâmetros PARAM chegam em rdi/rsi/rdx conforme sua posição na nova
 *      função. Mas ao montar os argumentos para f() precisamos mover valores entre
 *      esses mesmos registradores. Para não perder dados usamos r10/r11/r12 como
 *      temporários (callee-saved r12, caller-saved r10/r11 — salvo r12 se usado).
 *   3. Carregar cada argumento de f() no registrador destino correto:
 *      - FIX  int: mov $imm32, %reg  (ou movabs para 64-bit)
 *      - FIX  ptr: movabs $imm64, %reg
 *      - IND  int: movabs $addr64, %rax ; movl (%rax), %reg_32
 *      - IND  ptr: movabs $addr64, %rax ; mov  (%rax), %reg_64
 *      - PARAM   : o valor já está num temporário — mov %tmp, %reg
 *   4. Colocar endereço de f em %rax e call *%rax
 *   5. Epílogo: leave / ret
 *
 * Registradores destino para os 3 possíveis argumentos de f():
 *   pos 0 -> rdi   pos 1 -> rsi   pos 2 -> rdx
 *
 * Temporários para guardar os PARAM recebidos pela nova função:
 *   O i-ésimo PARAM recebido pela nova função chega em rdi (i=0), rsi (i=1) ou rdx (i=2).
 *   Guardamos respectivamente em: r10, r11, r12.
 *   Se r12 for usado como temporário, precisamos salvá-lo (callee-saved).
 */

/* ---- emissores de bytes ---- */
static int emit_byte(unsigned char *buf, int pos, unsigned char b)
{
    buf[pos] = b;
    return pos + 1;
}

static int emit_bytes(unsigned char *buf, int pos, const unsigned char *data, int len)
{
    int i;
    for (i = 0; i < len; i++)
        buf[pos + i] = data[i];
    return pos + len;
}

static int emit_imm32(unsigned char *buf, int pos, uint32_t v)
{
    buf[pos+0] = (v      ) & 0xFF;
    buf[pos+1] = (v >>  8) & 0xFF;
    buf[pos+2] = (v >> 16) & 0xFF;
    buf[pos+3] = (v >> 24) & 0xFF;
    return pos + 4;
}

static int emit_imm64(unsigned char *buf, int pos, uint64_t v)
{
    int i;
    for (i = 0; i < 8; i++)
        buf[pos+i] = (v >> (8*i)) & 0xFF;
    return pos + 8;
}

/* ---- movabs $imm64, %reg64 ----
 * REX.W (48) + B8+rd para rax..rdi, ou REX.W+REX.B (49) + B8+rd para r8..r15
 *
 * Registradores (encoding):
 *   rax=0, rcx=1, rdx=2, rbx=3, rsp=4, rbp=5, rsi=6, rdi=7
 *   r8=8,  r9=9,  r10=10, r11=11, r12=12
 */
static int emit_movabs_imm64_reg(unsigned char *buf, int pos, uint64_t imm, int reg)
{
    if (reg < 8) {
        /* REX.W */
        buf[pos++] = 0x48;
        buf[pos++] = 0xB8 + reg;
    } else {
        /* REX.W + REX.B */
        buf[pos++] = 0x49;
        buf[pos++] = 0xB8 + (reg - 8);
    }
    pos = emit_imm64(buf, pos, imm);
    return pos;
}

/* ---- mov %src64, %dst64 ----  (REX.W 89 /r  ou  REX.W 8B /r)
 * Usamos: REX.W + 0x89 + ModRM(11, src, dst)
 * ModRM = 11 | (src & 7) << 3 | (dst & 7)
 * Para r8-r15 precisamos de REX.R e/ou REX.B.
 */
static int emit_mov_reg_reg(unsigned char *buf, int pos, int src, int dst)
{
    unsigned char rex = 0x48; /* REX.W */
    if (src >= 8) rex |= 0x04; /* REX.R */
    if (dst >= 8) rex |= 0x01; /* REX.B */
    buf[pos++] = rex;
    buf[pos++] = 0x89;
    buf[pos++] = 0xC0 | ((src & 7) << 3) | (dst & 7);
    return pos;
}

/* ---- movl (%rax), %reg32  (carregar int de endereço em rax) ----
 * Para rdi/rsi/rdx/rcx/r8-r11:
 *   sem REX prefix para rdi(7), rsi(6), rdx(2): 8B /r com ModRM(00, reg, 000=rax)
 *   com REX.W para 64-bit; mas para INT_PAR carregamos 32 bits e zero-extend:
 *   MOV r32, [rax]  =  (REX.R se reg>=8) 0x8B ModRM(00, reg&7, 0)
 */
static int emit_mov_mem_rax_to_reg32(unsigned char *buf, int pos, int dst_reg)
{
    /* 8B /r  mem=[rax]  -- carrega 32 bits, zero-extends para 64 */
    if (dst_reg >= 8) {
        buf[pos++] = 0x44 | 0x00; /* REX.R */
        /* Na verdade: REX = 0100 0R0B, R=1 se dst>=8 */
        /* reescrever: */
        pos--;
        buf[pos++] = 0x44; /* REX com REX.R=1 */
    }
    buf[pos++] = 0x8B;
    buf[pos++] = (0x00) | ((dst_reg & 7) << 3) | 0x00; /* ModRM: mod=00, reg=dst&7, rm=0(rax) */
    return pos;
}

/* ---- mov (%rax), %reg64 ---- */
static int emit_mov_mem_rax_to_reg64(unsigned char *buf, int pos, int dst_reg)
{
    unsigned char rex = 0x48; /* REX.W */
    if (dst_reg >= 8) rex |= 0x04; /* REX.R */
    buf[pos++] = rex;
    buf[pos++] = 0x8B;
    buf[pos++] = (0x00) | ((dst_reg & 7) << 3) | 0x00; /* mod=00, rm=rax(0) */
    return pos;
}

/* ---- push %reg64 ---- */
static int emit_push_reg(unsigned char *buf, int pos, int reg)
{
    if (reg >= 8) buf[pos++] = 0x41; /* REX.B */
    buf[pos++] = 0x50 + (reg & 7);
    return pos;
}

/* ---- pop %reg64 ---- */
static int emit_pop_reg(unsigned char *buf, int pos, int reg)
{
    if (reg >= 8) buf[pos++] = 0x41; /* REX.B */
    buf[pos++] = 0x58 + (reg & 7);
    return pos;
}

/* ---- sub $imm8, %rsp ---- */
static int emit_sub_rsp_imm8(unsigned char *buf, int pos, unsigned char imm)
{
    buf[pos++] = 0x48; /* REX.W */
    buf[pos++] = 0x83;
    buf[pos++] = 0xEC; /* ModRM: /5 rsp */
    buf[pos++] = imm;
    return pos;
}

/* ---- add $imm8, %rsp ---- */
static int emit_add_rsp_imm8(unsigned char *buf, int pos, unsigned char imm)
{
    buf[pos++] = 0x48;
    buf[pos++] = 0x83;
    buf[pos++] = 0xC4; /* ModRM: /0 rsp */
    buf[pos++] = imm;
    return pos;
}

/* ==========================================================================
 * cria_func
 * ========================================================================== */

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    int pos = 0;

    /*
     * Registradores de argumento (índice = posição do argumento):
     *   0=rdi(7), 1=rsi(6), 2=rdx(2)
     */
    static const int arg_regs[3] = {7, 6, 2}; /* rdi, rsi, rdx */

    /*
     * Temporários para salvar PARAMs recebidos pela nova função.
     * Usamos r10(10), r11(11), r12(12).
     * r12 é callee-saved, portanto precisamos push/pop se usado.
     */
    static const int tmp_regs[3] = {10, 11, 12}; /* r10, r11, r12 */

    /*
     * Para cada posição de parâmetro PARAM recebido pela nova função,
     * precisamos saber em qual registrador de argumento ele chegou.
     * A nova função recebe somente os parâmetros PARAM, em ordem.
     * Ex.: se params = [FIX, PARAM, PARAM], a nova função recebe 2 args:
     *   1º arg da nova função -> chega em rdi -> salvo em r10
     *   2º arg da nova função -> chega em rsi -> salvo em r11
     */

    /* Conta quantos PARAMs existem e mapeia: posição no array -> índice de chegada */
    int param_count = 0;          /* quantos PARAM a nova função recebe */
    int param_recv_idx[3];        /* param_recv_idx[i] = índice de chegada do i-ésimo param */
    int param_tmp[3];             /* registrador temporário onde cada PARAM é guardado */
    int i;

    for (i = 0; i < n; i++) {
        if (params[i].orig_val == PARAM) {
            param_recv_idx[i] = param_count; /* chega no param_count-ésimo registrador */
            param_tmp[i]      = tmp_regs[param_count];
            param_count++;
        } else {
            param_recv_idx[i] = -1;
            param_tmp[i]      = -1;
        }
    }

    /* Verifica se r12 será usado (é callee-saved) */
    int use_r12 = 0;
    for (i = 0; i < n; i++)
        if (param_tmp[i] == 12) { use_r12 = 1; break; }

    /* ------------------------------------------------------------------
     * 1. PRÓLOGO
     *    push rbp
     *    mov  rbp, rsp
     *    (push r12 se necessário — callee-saved)
     *    sub  rsp, 8  para manter alinhamento de 16 bytes antes do call
     *
     *  A pilha ao entrar na nova função:
     *    [rsp] = endereço de retorno  (8 bytes)
     *  Após push rbp:
     *    [rsp] = rbp antigo, [rsp+8] = ret addr  -> rsp desalinhado em 8
     *  Após sub rsp, 8 (ou push r12):
     *    rsp alinhado em 16 antes do call
     * ------------------------------------------------------------------ */

    /* push rbp */
    pos = emit_byte(codigo, pos, 0x55);
    /* mov rbp, rsp */
    pos = emit_bytes(codigo, pos, (unsigned char[]){0x48, 0x89, 0xE5}, 3);

    /* push r12 se necessário (também ajuda no alinhamento) */
    if (use_r12) {
        pos = emit_push_reg(codigo, pos, 12);
        /* pilha agora desalinhada de 8 novamente? Vamos verificar:
         * entrada: rsp % 16 == 8  (ret addr na pilha)
         * push rbp:  rsp % 16 == 0
         * push r12:  rsp % 16 == 8  -> precisa de mais 8 bytes
         */
        pos = emit_sub_rsp_imm8(codigo, pos, 8);
    } else {
        /* Sem push r12: após push rbp, rsp % 16 == 0; sub 8 para desalinhar
         * e o call vai empurrar ret addr tornando rsp % 16 == 0 novamente. */
        pos = emit_sub_rsp_imm8(codigo, pos, 8);
    }

    /* ------------------------------------------------------------------
     * 2. SALVAR PARAMs recebidos em temporários (r10, r11, r12)
     *    Cada PARAM chega em arg_regs[param_recv_idx[i]] e vai para tmp_regs[...].
     * ------------------------------------------------------------------ */
    for (i = 0; i < n; i++) {
        if (params[i].orig_val == PARAM) {
            int src = arg_regs[param_recv_idx[i]];
            int dst = param_tmp[i];
            pos = emit_mov_reg_reg(codigo, pos, src, dst);
        }
    }

    /* ------------------------------------------------------------------
     * 3. CARREGAR ARGUMENTOS para f() nos registradores corretos
     * ------------------------------------------------------------------ */
    for (i = 0; i < n; i++) {
        int dst = arg_regs[i]; /* registrador destino do i-ésimo arg de f() */

        switch (params[i].orig_val) {

        case FIX:
            if (params[i].tipo_val == INT_PAR) {
                /* mov $imm32, %reg32  (zero-extend para 64 bits)
                 * Encoding: (REX.B se reg>=8) B8+rd  imm32
                 * Mas para rdi(7)/rsi(6)/rdx(2) sem REX.B:
                 *   rdi -> edi: B8+7 = 0xBF
                 *   rsi -> esi: B8+6 = 0xBE
                 *   rdx -> edx: B8+2 = 0xBA
                 */
                if (dst >= 8) {
                    codigo[pos++] = 0x41; /* REX.B */
                }
                codigo[pos++] = 0xB8 + (dst & 7);
                pos = emit_imm32(codigo, pos, (uint32_t)(uint64_t)params[i].valor.v_int);
            } else {
                /* PTR_PAR: movabs $imm64, %reg64 */
                pos = emit_movabs_imm64_reg(codigo, pos,
                                            (uint64_t)(uintptr_t)params[i].valor.v_ptr,
                                            dst);
            }
            break;

        case IND:
            /* Carregar endereço em rax, depois carregar valor de [rax] */
            pos = emit_movabs_imm64_reg(codigo, pos,
                                        (uint64_t)(uintptr_t)params[i].valor.v_ptr,
                                        0 /* rax */);
            if (params[i].tipo_val == INT_PAR) {
                pos = emit_mov_mem_rax_to_reg32(codigo, pos, dst);
            } else {
                pos = emit_mov_mem_rax_to_reg64(codigo, pos, dst);
            }
            break;

        case PARAM:
            /* O valor está no temporário param_tmp[i] */
            pos = emit_mov_reg_reg(codigo, pos, param_tmp[i], dst);
            break;
        }
    }

    /* ------------------------------------------------------------------
     * 4. CARREGAR ENDEREÇO de f em rax e call *rax
     * ------------------------------------------------------------------ */
    pos = emit_movabs_imm64_reg(codigo, pos, (uint64_t)(uintptr_t)f, 0 /* rax */);
    /* call *%rax: FF D0 */
    codigo[pos++] = 0xFF;
    codigo[pos++] = 0xD0;

    /* ------------------------------------------------------------------
     * 5. EPÍLOGO
     *    add rsp, 8      (desfaz o sub rsp,8 do prólogo)
     *    pop r12 se usado
     *    leave           (mov rsp,rbp ; pop rbp)
     *    ret
     * ------------------------------------------------------------------ */
    pos = emit_add_rsp_imm8(codigo, pos, 8);

    if (use_r12) {
        pos = emit_pop_reg(codigo, pos, 12);
    }

    /* leave */
    codigo[pos++] = 0xC9;
    /* ret */
    codigo[pos++] = 0xC3;
}