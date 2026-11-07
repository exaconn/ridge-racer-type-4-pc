#include "types.h"

#define SP ((Scr *)s2)

#define S4 ((Ent *)s4)

#define GTE_STORE_SXY_SZ(p) \
    __asm__ volatile( \
        "swc2 $12, 0(%0); swc2 $17, 4(%0); swc2 $13, 8(%0); swc2 $18, 12(%0); swc2 $14, 16(%0); swc2 $19, 20(%0)" \
        : : "r"(p) : "memory")

#define GTE_SETROT_AND_SAVE(src, dst) \
    __asm__ volatile( \
        "lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; sw $12, 0(%1); sw $13, 4(%1); " \
        "lw $12, 8(%0); lw $13, 12(%0); ctc2 $12, $2; ctc2 $13, $3; sw $12, 8(%1); " \
        "lw $12, 16(%0); sw $13, 12(%1); ctc2 $12, $4; sw $12, 16(%1)" \
        : : "r"(src), "r"(dst) : "$12", "$13", "memory")

#define GTE_CTC2_27(val) \
    __asm__ volatile("ctc2 %0, $27" : : "r"(val))

#define GTE_CTC2_28(val) \
    __asm__ volatile("ctc2 %0, $28" : : "r"(val))

/* ASM_BARRIER: empty asm that only reads its operands; keeps both values live to the end of the block
 * so gcc expands the signed divisions with a copy (bgez; move; addiu) as in the original. */
#define KEEP_LIVE2(a, b) __asm__ volatile("" : : "r"(a), "r"(b))

#define GTE_LWC2_IR0(p) __asm__ volatile("lwc2 $8, 0(%0)" : : "r"(p))

#define GTE_LWC2_IR0_MEM(p) __asm__ volatile("lwc2 $8, 0(%0)" : : "r"(p) : "memory")

#define GTE_DPCS_COL(col, dc) __asm__ volatile("mtc2 %1, $8\n\tmtc2 %0, $6\n\tnop\n\tnop\n\t.word 0x4A780010\n\tmfc2 %0, $22" : "+r"(col) : "r"(dc))

#define GTE_MTC2_RGB_DPCS(c) __asm__ volatile("mtc2 %0, $6\n\tnop\n\tnop\n\t.word 0x4A780010" : : "r"(c))

/* legacy typing of shared globals in this part of the unit */
#define PsyqRand ((int (*)())PsyqRand)

#undef PsyqRand

extern s32 ModelTexturePage;

void SetModelTexturePage(s32 arg0) {
    ModelTexturePage = arg0;
}
