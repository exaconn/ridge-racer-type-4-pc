/* CC1_FLAGS: -mno-split-addresses */
#include "rr/sequencer_dispatch.h"
#include "psyq/libsnd/vmtick2.h"
#include "psyq/libsnd/ssramp.h"
#include "psyq/libsnd/ssresum2.h"
#include "psyq/libsnd/ssresume.h"
#include "psyq/libsnd/sstick3.h"
extern void _SsSndTempo(s32,s32);
void SsSeqCalledTbyT(void) {
    s32 bank, offset, signed_row;
    s32 row;
    s32 row_shift;
    s32 bank_shift;
    s32 signed_bank;
    RrLibStateB0 **entry,**records;
    s32 row_limit,bank_limit;
    if(_snd_ev_flag!=1) {
        _snd_ev_flag=1;
        bank=0;
        _SsVmFlush();
        if(_snd_seq_s_max>0) {
            records=_ss_score;
            do {
                if(_snd_openflag&(1U<<bank)) {
                    row=0;
                    if(_snd_seq_t_max>0) {
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
                                _SsSndPlay(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x10)func_80082FC4(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x20)func_80082FC4(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x40)_SsSndTempo(signed_bank,signed_row);
                                if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&0x80)_SsSndTempo(signed_bank,signed_row);
                            }
                            if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&2)_SsSndPause(bank_shift>>16,row_shift>>16);
                            if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&8)_SsSndReplay(bank_shift>>16,row_shift>>16);
                            if(((RrLibStateB0*)(offset+(unsigned long)*entry))->w98&4) {
                                _SsSndStop(bank_shift>>16,row_shift>>16);
                                ((RrLibStateB0*)(offset+(unsigned long)*entry))->w98=0;
                            }
                            row_shift+=0x10000;
                            row_limit=_snd_seq_t_max;
                            /* ASM_BARRIER: Read the live row bound before incrementing its counter. */
                            __asm__("" : "+r"(row_limit));
                            row++;
                            offset+=sizeof(RrLibStateB0);
                        }while(row<row_limit);
                    }
                }
                bank_limit=_snd_seq_s_max;
                /* ASM_BARRIER: Read the live bank bound before incrementing its counter. */
                __asm__("" : "+r"(bank_limit));
                bank++;records++;
            }while(bank<bank_limit);
        }
        _snd_ev_flag=0;
    }
}
