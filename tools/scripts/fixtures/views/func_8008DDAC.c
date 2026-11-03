/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/cd.h"

#include "rr/library.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800A6764 (*(u8 (*)[2])&D_800A6764)

#undef D_800A6764

extern volatile u8 D_800A6764[];

extern s32 func_8008C724(void);

void func_8008DDAC(void) {
    u8 *p=&D_800A6764[1];
    u8 *q=p-1;
    s32 saved=*D_800A674C & 3;
    s32 result;
    while((result=func_8008C724())) {
        if(result & 4) {
            if(D_800A648C) {
                /* ASM_BARRIER: Load the byte argument before preparing its data-page argument. */
                register s32 argument=*p;
                /* ASM_BARRIER: Retain the callback's data-page argument for its call delay slot. */
                register u8 *page __asm__("$5");
                /* ASM_BARRIER: Materialize the callback data page only on its active path. */
                __asm__ volatile("" : : "r"(argument) : "$5");
                page=(u8 *)0x80110000U;
                /* ASM_BARRIER: Keep the data-page offset separate for call scheduling. */
                __asm__("" : "+r"(page));
                D_800A648C(argument,page+0x320);
            }
        }
        if(result & 2) {
            if(D_800A6488) {
                /* ASM_BARRIER: Load the byte argument before preparing its data-page argument. */
                register s32 argument=*q;
                /* ASM_BARRIER: Retain the callback's data-page argument for its call delay slot. */
                register u8 *page __asm__("$5");
                /* ASM_BARRIER: Materialize the callback data page only on its active path. */
                __asm__ volatile("" : : "r"(argument) : "$5");
                page=(u8 *)0x80110000U;
                /* ASM_BARRIER: Keep the data-page offset separate for call scheduling. */
                __asm__("" : "+r"(page));
                D_800A6488(argument,page+0x318);
            }
        }
    }
    *D_800A674C=saved;
}
