/* CC1_FLAGS: -mno-split-addresses -fno-strength-reduce */
#include "rr/sequencer_init.h"

void SsSetTableSize(RrLibStateB0 *base,s32 raw_banks,s32 raw_rows) {
    s32 bank;
    s32 rows;
    s32 index;
    s32 offset;
    s32 raw_bank_copy;
    /* ASM_BARRIER: retain the raw bank argument in a1 for in-place narrowing. */
    register s32 bank_argument __asm__("$5")=raw_banks;
    s32 step;
    s32 count_shift;
    RrLibStateB0 **first_entry;
    RrLibStateB0 **entry;
    s32 minus_one;
    s32 maximum;
    RrLibStateB0 *last;
    s32 row_shift;
    s32 bank_limit;
    RrLibStateB0 **records;
    raw_bank_copy=bank_argument;
    _snd_seq_s_max=bank_argument;
    /* ASM_BARRIER: Publish raw bank low16 before narrowing a1; preserve its raw copy for the mask. */
    __asm__("" : "+r"(bank_argument) : "r"(raw_bank_copy) : "memory");
    bank_argument=(s16)bank_argument;
    _snd_seq_t_max=raw_rows;
    bank=0;
    if(bank_argument>0) {
        /* ASM_BARRIER: Keep the bank loop index distinct from the later zero record index. */
        __asm__("" : "+r"(bank));
        row_shift=(u32)raw_rows<<16;
        step=row_shift>>16;
        first_entry=_ss_score;
        index=0;
        do {
            *first_entry++=base+index;
            bank++;
            index+=step;
        }while(bank<bank_argument);
    }
    count_shift=(u32)raw_bank_copy<<16;
    bank=count_shift>>16;
    while(bank<32) {
        _snd_openflag|=1U<<bank;
        bank++;
    }
    bank=0;
    if(_snd_seq_s_max>0) {
        minus_one=-1;maximum=127;
        records=_ss_score;
        do {
            rows=0;
            if(_snd_seq_t_max>0) {
                /* ASM_BARRIER: Keep the row index distinct from the independent byte offset. */
                __asm__("" : "+r"(rows));
                entry=records;offset=0;
                do {
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->w98=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->b22[0]=minus_one;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->b22[1]=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->h48[0]=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->h48[1]=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->w9c=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->wa0=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->h48[2]=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->wac=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->wa8=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->wa4=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->h48[3]=0;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->h58[0]=maximum;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->h58[1]=maximum;
                    rows++;
                    ((RrLibStateB0*)(offset+(unsigned long)*entry))->h58[2]=maximum;
                    last=(RrLibStateB0*)(offset+(unsigned long)*entry);
                    offset+=sizeof(*last);
                    last->h58[3]=maximum;
                }while(rows<_snd_seq_t_max);
            }
            bank_limit=_snd_seq_s_max;
            /* ASM_BARRIER: Read the current bank limit before incrementing the outer index. */
            __asm__("" : "+r"(bank_limit));
            bank++;records++;
        }while(bank<bank_limit);
    }
}
