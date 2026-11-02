/* CC1_FLAGS: -mno-split-addresses */
#include "rr/sequencer_dispatch.h"
extern void func_80087064(void);
extern void func_80083274(s16,s16);
extern void func_80082FC4(s32,s32);
extern void func_80084314(s32,s32);
extern void func_800831D4(s32,s32);
extern void func_80083A94(s32,s16);
extern void func_80083AF4(s32,s32);
void func_80082D54(void) {
    s32 bank, offset, signed_row;
    s32 row;
    s32 row_shift;
    s32 bank_shift;
    s32 signed_bank;
    RrLibStateB0 **entry,**records;
    s32 row_limit,bank_limit;
    if(D_8010F05C!=1) {
        D_8010F05C=1;
        bank=0;
        func_80087064();
        if(D_8010F8E8>0) {
            records=D_8010F068;
            do {
                if(D_8010F060&(1U<<bank)) {
                    row=0;
                    if(D_8010F8EA>0) {
                        /* ASM_BARRIER: Keep the row counter distinct from the later shifted row and byte offset. */
                        __asm__("" : "+r"(row));
                        entry=records;
                        bank_shift=(u32)bank<<16;
                        signed_bank=bank_shift>>16;
                        /* ASM_BARRIER: Prepare both bank representations before starting the per-record state. */
                        __asm__("" : : "r"(bank_shift),"r"(signed_bank));
                        row_shift=0;
                        /* ASM_BARRIER: Keep shifted row zero independent from the separate byte-offset zero. */
                        __asm__("" : "+r"(row_shift));
                        offset=0;
                        do {
                            if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&1) {
                                signed_row=row_shift>>16;
                                func_80083274(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x10)func_80082FC4(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x20)func_80082FC4(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x40)func_80084314(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x80)func_80084314(signed_bank,signed_row);
                            }
                            if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&2)func_800831D4(bank_shift>>16,row_shift>>16);
                            if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&8)func_80083A94(bank_shift>>16,row_shift>>16);
                            if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&4) {
                                func_80083AF4(bank_shift>>16,row_shift>>16);
                                ((RrLibStateB0*)(offset+(unsigned long)*entry))->w98=0;
                            }
                            row_shift+=0x10000;
                            row_limit=D_8010F8EA;
                            /* ASM_BARRIER: Read the live row bound before incrementing its counter. */
                            __asm__("" : "+r"(row_limit));
                            row++;
                            offset+=sizeof(RrLibStateB0);
                        }while(row<row_limit);
                    }
                }
                bank_limit=D_8010F8E8;
                /* ASM_BARRIER: Read the live bank bound before incrementing its counter. */
                __asm__("" : "+r"(bank_limit));
                bank++;records++;
            }while(bank<bank_limit);
        }
        D_8010F05C=0;
    }
}
