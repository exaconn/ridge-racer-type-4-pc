/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

LibInterruptCallback setIntrDMA(s32 channel,LibInterruptCallback callback) {
    s32 i=channel;
    LibInterruptCallback *entry=InterruptCallbackTable;
    /* ASM_BARRIER: Retain the previous callback for the common return. */
    register LibInterruptCallback old __asm__("$7");
    LibInterruptCallback value;
    /* ASM_BARRIER: Keep the device word base in the consumed argument register. */
    register volatile u32 *control __asm__("$5");
    LibInterruptCallback result;
    entry=(LibInterruptCallback *)((unsigned long)entry+(u32)i*sizeof(*entry));
    old=*entry;
    value=callback;
    result=old;
    if(value!=old) {
        u32 mask;
        mask=0xFF0000;
        if(value) {
            u32 word,bit;
            s32 shift;
            control=DICR_VSync;
            mask|=65535;
            *entry=value;
            word=*control;
            shift=i+16;
            word&=mask;
            bit=(1U<<shift)|0x800000;
            *control=word|bit;
        } else {
            u32 word;
            s32 shift;
            control=DICR_VSync;
            mask|=65535;
            *entry=0;
            word=*control;
            shift=i+16;
            word&=mask;
            word|=0x800000;
            *control=word&~(1U<<shift);
        }
        result=old;
        /* ASM_BARRIER: Keep the changed-callback return separate from the equality gate. */
        __asm__("" : "+r"(result));
    }
    return result;
}
