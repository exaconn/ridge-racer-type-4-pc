/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"
s32 func_8009D978(void) {
    register s32 result;
    /* ASM_BARRIER: Keep the folding accumulator separate from the returned byte. */
    register s32 checksum __asm__("$3");
    if(D_800ABDF0) {
        /* ASM_BARRIER: Keep the retry-phase comparison after its independent tick read. */
        __asm__("" : : : "$2");
        switch(D_800ABDE8) {
        case 2: result=0x52;goto done;
        case 5: goto high;
        case 6: goto low;
        default: result=0;goto done;
        }
    }
    /* ASM_BARRIER: Keep the normal-phase comparison after its independent tick read. */
    __asm__("" : : : "$2");
    switch(D_800ABDE8) {
    case 2: D_80114710=0;result=0x57;goto done;
    case 5:
high: result=((u8 *)&D_80114724)[1];goto done;
    case 6:
low: result=*(u8 *)&D_80114724;goto done;
    case 133: result=D_8011472C.value;checksum=D_80114710^result;goto fold;
    case 134: result=D_80114710;goto done;
    case 135: result=((D_80114724>>8)^D_80114724)&255;goto done;
    default:
        if(D_800ABDE8<7) {result=0;goto done;}
        /* ASM_BARRIER: Preserve the separate fresh tick read for the upper bound. */
        __asm__("" : : : "memory");
        if(D_800ABDE8>=133) {result=0;goto done;}
        /* ASM_BARRIER: Reload the selected tick after both bounds have passed. */
        __asm__("" : : : "memory");
        {
            s32 phase=D_800ABDE8;
            checksum=D_80114710;
            result=D_80114740[phase+3];
            checksum^=result;
        }
fold:
        D_80114710=checksum;
    }
done:
    /* ASM_BARRIER: Retain the original shared return entry for all protocol phases. */
    __asm__("" : "+r"(result));
    return result;
}
