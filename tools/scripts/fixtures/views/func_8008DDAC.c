/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/cd.h"

#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define CD_intr_status (*(u8 (*)[2])&CD_intr_status)

#undef CD_intr_status

extern volatile u8 CD_intr_status[];

extern s32 getintr(void);

void CdCommandCallback(void) {
    u8 *p=&CD_intr_status[1];
    u8 *q=p-1;
    s32 saved=*CD_reg0 & 3;
    s32 result;
    while((result=getintr())) {
        if(result & 4) {
            if(CD_cbready) {
                /* ASM_BARRIER: Load the byte argument before preparing its data-page argument. */
                register s32 argument=*p;
                /* ASM_BARRIER: Retain the callback's data-page argument for its call delay slot. */
                register u8 *page __asm__("$5");
                /* ASM_BARRIER: Materialize the callback data page only on its active path. */
                __asm__ volatile("" : : "r"(argument) : "$5");
                page=(u8 *)0x80110000U;
                /* ASM_BARRIER: Keep the data-page offset separate for call scheduling. */
                __asm__("" : "+r"(page));
                CD_cbready(argument,page+0x320);
            }
        }
        if(result & 2) {
            if(CD_cbsync) {
                /* ASM_BARRIER: Load the byte argument before preparing its data-page argument. */
                register s32 argument=*q;
                /* ASM_BARRIER: Retain the callback's data-page argument for its call delay slot. */
                register u8 *page __asm__("$5");
                /* ASM_BARRIER: Materialize the callback data page only on its active path. */
                __asm__ volatile("" : : "r"(argument) : "$5");
                page=(u8 *)0x80110000U;
                /* ASM_BARRIER: Keep the data-page offset separate for call scheduling. */
                __asm__("" : "+r"(page));
                CD_cbsync(argument,page+0x318);
            }
        }
    }
    *CD_reg0=saved;
}
