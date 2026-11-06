/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

extern s32 func_80096E64(u32,s32);

extern s32 D_800A4828;

extern void _spu_Fw1ts(void), func_8007F574(void);

void _spu_FiDMA(void) {
    /* ASM_BARRIER: Retain the bounded poll count in its original register. */
    register u32 i;
    RrSpuRegisterBank *bank;
    if(D_800A4828==0) _spu_Fw1ts();
    bank=D_800A47D8;
    bank->spu_control &= 0xFFCF;
    {
    u32 status=bank->spu_control & 0x30;
    if(status) {
        /* ASM_BARRIER: Preserve the initial zero in the first status branch delay slot. */
        __asm__("" : "=r"(i) : "0"(0));
        i++;
        /* ASM_BARRIER: Keep the loop backedge after the initial counter increment. */
        __asm__ volatile("" : "=r"(i) : "0"(i));
again:
        if(i<0xF01) {
            if(bank->spu_control & 0x30) {
                i++;
                goto again;
            }
        }
    }
    }
    if(D_800A4810) D_800A4810();
    else func_80096E64(0xF0000009U,0x20);
}
