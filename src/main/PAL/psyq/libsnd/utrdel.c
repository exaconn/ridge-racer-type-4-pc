/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "rr/library.h"
#include "m2c_macros.h"
#include "psyq/libsnd/utrev.h"
#include "psyq/libspu/srevattr.h"

extern int D_80110008[];
extern s16 D_8011000C;

s16 func_80086680(void);

s32 func_800865E4(s32 a0) {
    register int v1 __asm__("$3"); /* ASM_BARRIER: original keeps the sign-adjusted copy in v1, not a0 */
    short s;
    /* ASM_BARRIER: Retain the narrowing shift independently of its source. */
    register u32 shifted __asm__("$2");
    int a1;
    a1 = 0;
    v1 = a0;
    if ((short)a0 < 0) { a1 = 1; v1 = (s32)(0U - (u32)a0); }
    if ((unsigned short)v1 < 10) {
        D_80110008[0] = 1;
        if (a1 != 0) D_80110008[1] = (short)(v1 | 0x100);
        else D_80110008[1] = (short)v1;
        shifted=(u32)v1<<16;
        /* ASM_BARRIER: Retain the shifted temporary in v0 before signed narrowing. */
        __asm__("" : "+r"(shifted));
        s=(s32)shifted>>16;
        if (s == 0) func_80086694(0);
        {
            s32 *page;
            /* ASM_BARRIER: Finish the optional reset before preparing its shared command argument. */
            __asm__ volatile("" : : : "$4");
            page=(s32 *)0x80110000U;
            /* ASM_BARRIER: Materialize the command page before the call-delay offset. */
            __asm__("" : "+r"(page));
            func_80085BA4(page+2);
        }
        return s;
    }
    return -1;
}

s16 func_80086680(void) {
    return D_8011000C;
}
