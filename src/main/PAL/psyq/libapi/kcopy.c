#include "rr/memory.h"
extern void func_80095DF4(void);
extern void func_80095E74(void);
void func_80095FA0(void) {
    /* ASM_BARRIER: Retain the kernel destination in the original cursor register. */
    register unsigned long destination __asm__("$2")=0xDF80U;
    /* ASM_BARRIER: Retain the source code cursor in the original register. */
    register unsigned long source __asm__("$10")=(unsigned long)func_80095DF4+0x10;
    /* ASM_BARRIER: Keep the source endpoint in its original register. */
    register unsigned long limit __asm__("$9")=(unsigned long)func_80095E74;
    do {
        *(u32 *)destination=*(const u32 *)source;
        /* ASM_BARRIER: Publish each code word before advancing the source cursor. */
        __asm__ volatile("" : : "r"(source) : "memory");
        source+=sizeof(u32);
        destination+=sizeof(u32);
    } while(source!=limit);
}
