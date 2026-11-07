/* GCC_VERSION: 2.7.2 */
#include "rr/spu.h"
extern void _spu_Fw1ts(void);
extern s32 DeliverEvent(u32,s32);
extern s32 _spu_dmaIsRead;
extern void (*volatile _spu_transferCallback)(void);
void _spu_FiDMA(void) {
    /* ASM_BARRIER: Retain the bounded poll count in its original register. */
    register u32 i;
    RrSpuRegisterBank *bank;
    if(_spu_dmaIsRead==0) _spu_Fw1ts();
    bank=_spu_RXX;
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
    if(_spu_transferCallback) _spu_transferCallback();
    else DeliverEvent(0xF0000009U,0x20);
}
