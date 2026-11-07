/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu_key.h"
#define HW (((RrSpuRegisterStorage *)_spu_RXX)->halves)
void SpuSetKey(s32 mode,u32 mask) {
    u32 high;
    mask&=0xFFFFFF;
    high=mask>>16;
    switch(mode) {
    case 1: {
        if(D_800A48A4&1) {
            volatile u16 *pending=D_8010EFB0;
            pending[0]=mask;
            pending[1]=high;
            /* Preserve the observed ordering of queued cache updates. */
            __asm__("" : : : "memory");
            D_800A4870|=1;
            /* Preserve the observed ordering of queued cache updates. */
            __asm__("" : : : "memory");
            D_800A486C|=mask;
            /* Preserve the observed ordering of queued cache updates. */
            __asm__("" : : : "memory");
            if(pending[2]&mask)pending[2]&=~mask;
            if(pending[3]&high)pending[3]&=~high;
        } else {
            u32 active=_spu_keystat|mask;
            volatile u16 *bank=HW;
            bank[196]=mask;
            bank[197]=high;
            _spu_keystat=active;
        }
        break;
    }
    case 0: {
        if(D_800A48A4&1) {
            volatile u16 *pending=D_8010EFB0;
            pending[2]=mask;
            pending[3]=high;
            /* Preserve the observed ordering of queued cache updates. */
            __asm__("" : : : "memory");
            D_800A4870|=1;
            /* Preserve the observed ordering of queued cache updates. */
            __asm__("" : : : "memory");
            D_800A486C&=~mask;
            /* Preserve the observed ordering of queued cache updates. */
            __asm__("" : : : "memory");
            if(pending[0]&mask)pending[0]&=~mask;
            if(pending[1]&high)pending[1]&=~high;
        } else {
            volatile u16 *bank=HW;
            bank[198]=mask;
            bank[199]=high;
            _spu_keystat&=~mask;
        }
        break;
    }
    }
    /* All paths finish after their memory updates with the initialized mask. */
    __asm__ volatile("" : : "r"(mask),"r"(high) : "memory");
}
