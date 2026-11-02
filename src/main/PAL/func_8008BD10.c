/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
RrLibInterruptCallback func_8008BD10(s32 channel,RrLibInterruptCallback callback) {
    s32 i=channel;
    RrLibInterruptCallback *entry=D_800A63A4;
    /* ASM_BARRIER: Retain the previous callback for the common return. */
    register RrLibInterruptCallback old __asm__("$7");
    RrLibInterruptCallback value;
    /* ASM_BARRIER: Keep the device word base in the consumed argument register. */
    register volatile u32 *control __asm__("$5");
    RrLibInterruptCallback result;
    entry=(RrLibInterruptCallback *)((unsigned long)entry+(u32)i*sizeof(*entry));
    old=*entry;
    value=callback;
    result=old;
    if(value!=old) {
        u32 mask;
        mask=0xFF0000;
        if(value) {
            u32 word,bit;
            s32 shift;
            control=D_800A63A0;
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
            control=D_800A63A0;
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
