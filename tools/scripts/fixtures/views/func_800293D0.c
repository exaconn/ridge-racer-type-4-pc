#include "rr/record_banks.h"

void func_800293D0(s32 layout,s32 mode) {
    /* ASM_BARRIER: keep each loop's index in the consumed mode register. */
    register s32 i __asm__("$5");
    register u32 factor;
    register u8 *base;
    /* ASM_BARRIER: finish each shared arena address before copying its base. */
    register u8 *loaded __asm__("$2");
    register u8 *bankbase;
    register RrBankRecord320 **source_slots;
    register u32 offset;
    register RrBankRecord320 **bank_source_slots;
    register u8 **bank_slots;
    register u32 bank_offset;
    RrBankRecord320 **restore_source,**restore_dest;
    s32 bound;
    register u8 *read_page;
    u8 *bound_page;

    D_800AC5A8=0;
    D_800AD504=mode;
    if (mode==2) {
        /* PAL mapping: page-0x3A64 is the live signed count D_800AC59C. */
        read_page=(u8 *)0x800B0000U;
        /* ASM_BARRIER: capture the count page before testing and retaining it. */
        __asm__("" : "+r"(read_page));
        i=0;
        if (*(s16 *)(read_page-0x3A64)>0) {
            bound_page=read_page;
            restore_dest=CarTable;
            restore_source=D_800AC570;
            do {
                **restore_dest=**restore_source;
                if ((*restore_dest)->value1E4>=17) {
                    (*restore_dest)->value1E4&=255;
                }
                restore_dest++;
                bound=*(s16 *)(bound_page-0x3A64);
                /* ASM_BARRIER: reload the limit before incrementing the index. */
                __asm__("" : "+r"(bound));
                i++;
                restore_source++;
            } while (i<bound);
        }
    } else {
        D_800ADAC8=0;
        i=0;
        if (layout==1) {
            loaded=D_800FFC48;
            base=loaded;
            source_slots=D_800AC570;
            offset=0x5468;
            D_80107188=2700;
            D_800AD510[0]=base;
            D_800AD510[1]=base+0x2A34;
            do {
                *source_slots=(RrBankRecord320 *)((unsigned long)offset+
                                                (unsigned long)base);
                source_slots++;
                i++;
                offset+=0x320;
            } while (i<4);
        } else if (layout==2) {
            D_80107188=900;
            /* ASM_BARRIER: republish zero after this branch's metadata store;
             * the previous fixed-register index value is dead at this point. */
            __asm__("" : : : "$5");
            i=0;
            factor=901;
            bankbase=D_800FFC48;
            bank_source_slots=D_800AC570;
            bank_offset=0x3850;
            bank_slots=D_800AD510;
            do {
                register u32 scaled=(u32)i*4U;
                u32 product;
                product=factor*scaled;
                *bank_slots=(u8 *)((unsigned long)product+(unsigned long)bankbase);
                *bank_source_slots=(RrBankRecord320 *)((unsigned long)bank_offset+
                                                     (unsigned long)bankbase);
                bank_source_slots++;
                bank_offset+=0x320;
                i++;
                bank_slots++;
            } while (i<4);
        } else {
            loaded=D_800FFC48;
            base=loaded;
            source_slots=D_800AC570;
            offset=0x3844;
            D_80107188=3600;
            D_800AD510[0]=base;
            do {
                *source_slots=(RrBankRecord320 *)((unsigned long)offset+
                                                (unsigned long)base);
                source_slots++;
                i++;
                offset+=0x320;
            } while (i<8);
        }
    }
}
