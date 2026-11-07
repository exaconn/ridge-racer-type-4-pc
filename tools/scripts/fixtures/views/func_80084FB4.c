/* CC1_FLAGS: -mno-split-addresses */

#include "rr/spu.h"

s32 SsUtGetProgAtr(s32 raw_bank, s16 program, u8 *output) {
    s32 offset;
    /* ASM_BARRIER: Keep the incoming output pointer until the bank gate is resolved. */
    register u8 *incoming __asm__("$6")=output;
    register s32 bank __asm__("$4")=raw_bank;
    register unsigned long page __asm__("$3");
    /* ASM_BARRIER: Preserve the raw input before narrowing and saving the output register. */
    __asm__("" : "+r"(bank) : : "$17");
    bank=(s16)bank;
    /* ASM_BARRIER: Finish signed bank normalization before creating its flag page. */
    __asm__("" : : "r"(bank) : "$3");
    page=0x80110000U;
    /* ASM_BARRIER: Expose the actual PAL page and signed byte offset. */
    __asm__("" : "+r"(page));
    page=*(u8 *)(page+(long)bank+0x120);
    /* ASM_BARRIER: Capture the output pointer after the flag read for either branch. */
    __asm__("" : "+r"(incoming) : : "memory");
    output=incoming;
    if(page==1) {
        /* Selection failure is intentionally ignored; copy from the current table. */
        _SsVmVSetUp(bank, program);
        offset=program*16;
        output[0]=*(u8 *)(offset+(unsigned long)_svm_pg);
        output[1]=((u8 *)(offset+(unsigned long)_svm_pg))[1];
        output[2]=((u8 *)(offset+(unsigned long)_svm_pg))[2];
        output[3]=((u8 *)(offset+(unsigned long)_svm_pg))[3];
        output[4]=((u8 *)(offset+(unsigned long)_svm_pg))[4];
        *(u16 *)(output+6)=*(u16 *)(offset+(unsigned long)_svm_pg+6);
        return 0;
    }
    return -1;
}
