#include "rr/memory.h"
u8 *PsyqMemmove(u8 *destination,const u8 *source,s32 length) {
    /* ASM_BARRIER: Preserve the original destination-walking register. */
    register u8 *cursor=destination;
    /* ASM_BARRIER: Preserve the original signed length test register. */
    register s32 test_length;
    /* ASM_BARRIER: Preserve the original loop comparison register. */
    /* ASM_BARRIER: Preserve the original register through address or return preparation. */
    register s32 previous __asm__("$3");
    u32 remaining=length;
    register u8 *destination_byte;
    register const u8 *source_byte;
    /* ASM_BARRIER: Preserve the original register through address or return preparation. */
    register u8 *answer __asm__("$2");
    if(cursor>=source) {
        test_length=length;
        /* ASM_BARRIER: Keep the signed length gate distinct from the address comparison. */
        __asm__ volatile("" : "=r"(test_length) : "0"(test_length));
        if(test_length<=0) goto done;
        remaining--;
        do {
            destination_byte=cursor+remaining;
            /* ASM_BARRIER: Form the destination address before reusing v0 for the source. */
            __asm__("" : "+r"(destination_byte) : : "$2");
            source_byte=source+remaining;
            /* ASM_BARRIER: Complete both addresses before snapshotting the loop comparison. */
            __asm__("" : "+r"(source_byte) : "r"(destination_byte) : "$3");
            previous=remaining;
            *destination_byte=*source_byte;
            remaining--;
        } while(previous>0);
        goto done;
    }
    test_length=length;
    /* ASM_BARRIER: Keep the signed length gate distinct from the address comparison. */
    __asm__ volatile("" : "=r"(test_length) : "0"(test_length));
    if(test_length<=0) goto done;
    remaining--;
    do {
        *cursor++=*source++;
        previous=remaining;
        remaining--;
    } while(previous>0);
done:
    answer=cursor;
    /* ASM_BARRIER: Preserve the shared return site after selecting the resulting cursor. */
    __asm__ volatile("" : : "r"(answer));
    return answer;
}
