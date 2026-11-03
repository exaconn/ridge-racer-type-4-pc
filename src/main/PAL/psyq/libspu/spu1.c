/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"

extern s32 func_80096E64(u32,s32);
extern s32 D_800A4828;
extern volatile int *D_800A47DC;
extern volatile int *D_800A47E0;
extern volatile int *D_800A47E4;
extern void func_8007F59C(void), func_8007F574(void);

void func_8007EF04(void);
void func_8007EFC0(int a0, int a1, int a2);

void func_8007EF04(void) {
    /* ASM_BARRIER: Retain the bounded poll count in its original register. */
    register u32 i;
    RrSpuRegisterBank *bank;
    if(D_800A4828==0) func_8007F59C();
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

void func_8007EFC0(int a0, int a1, int a2) {
    D_800A47D8->transfer_address = a1;
    func_8007F59C();
    D_800A47D8->spu_control |= 0x30;
    func_8007F59C();
    func_8007F574();
    *D_800A47DC = a0;
    *D_800A47E0 = ((u32)a2 << 16) | 0x10;
    D_800A4828 = 1;
    *D_800A47E4 = 0x1000200;
}
