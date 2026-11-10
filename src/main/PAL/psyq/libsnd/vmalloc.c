/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channel_voice_choice.h"
#include "rr/spu_voice_cache.h"

s32 _SsVmAlloc(void) {
    u32 chosen;
    /* ASM_BARRIER: retain the captured selection accumulator in its original temporary. */
    register u32 best_gate asm("$12");
    s32 ties;
    u32 best_age;
    /* ASM_BARRIER: retain the captured selection accumulator in its original temporary. */
    register u32 best asm("$9");
    u32 index;
    s32 priority;
    s32 limit;
    u32 reserved;
    u32 one;
    s32 offset;
    s32 signed_priority;
    /* ASM_BARRIER: retain the captured selection accumulator in its original temporary. */
    register u32 raw_priority asm("$4");
    s32 cutoff;
    u32 gate;
    u32 age;
    u32 initial;
    s32 count;
    u32 ix;
    s32 tmp;
    u32 ties_test;
    s32 loop_test;
    s32 signed_age;
    /* ASM_BARRIER: retain shared field page temporaries in their original registers. */
    register u8 *at_page asm("$1");
    /* ASM_BARRIER: retain shared field page temporary in A2. */
    register u8 *a2_page asm("$6");
    /* ASM_BARRIER: retain shared field page temporary in T0. */
    register u8 *t0_page asm("$8");
    chosen=99;
    best_gate=65535;
    ties=0;
    /* ASM_BARRIER: initialize independent candidate accumulators before scanning. */
    __asm__("" : "+r"(ties), "+r"(chosen));
    best_age=0;
    /* ASM_BARRIER: retain the independent initial age accumulator. */
    __asm__("" : "+r"(best_age));
    best=99;
    index=0;
    /* ASM_BARRIER: keep the byte loop comparison separate from the next byte index. */
    __asm__("" : "+r"(index));
    initial=D_8011010F;
    count=spuVmMaxVoice;
    /* ASM_BARRIER: capture the priority byte and active count before narrowing priority. */
    __asm__("" : : : "memory");
    priority=(s8)initial;
    if(count>0) {
        one=1;
        reserved=_snd_vmask;
        limit=count;
        ix=(u8)index;
        do {
            if(!(reserved&(one<<ix))) {
                tmp=ix*(sizeof(ChannelControlRow36)/2);
                offset=tmp*2;
                if(( {
                    u8 *page=(u8 *)0x80110000U;
                    /* ASM_BARRIER: capture the PAL row page before applying its index and field offset. */
                    __asm__("" : "+r"(page));
                    page+=( offset );
                    *( s8 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> flags1D );
                }
                ) ==0 && ( {
                    u8 *page=(u8 *)0x80110000U;
                    /* ASM_BARRIER: capture the PAL row page before applying its index and field offset. */
                    __asm__("" : "+r"(page));
                    page+=( offset );
                    *( u16 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> value06 );
                }
                ) ==0) {
                    chosen=index;
                    break;
                }
                offset=(u8)index*sizeof(ChannelControlRow36);
                cutoff=(u16)priority;
                signed_priority= ( {
                    a2_page=(u8 *)0x80110000U;
                    a2_page+=offset;
                    *( s16 *)((u8 *)&((ChannelControlRow36 *)(a2_page-0x6F8))-> priority1A );
                }
                );
raw_priority= ( {
                    /* ASM_BARRIER: retain this field page in its original temporary until the row access. */
                    register u8 *page asm("$4")=(u8 *)0x80110000U;
page+=( offset );
                    *( u16 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> priority1A );
                }
                );
                if(signed_priority<cutoff) {
                    priority=raw_priority;
                    best=index;
                    best_gate= ( {
                        /* ASM_BARRIER: retain this field page in its original temporary until the row access. */
                        register u8 *page asm("$12")=(u8 *)0x80110000U;
page+=( offset );
                        *( u16 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> value06 );
                    }
                    );
best_age= ( {
                    t0_page=(u8 *)0x80110000U;
                    t0_page+=offset;
                        *( u16 *)((u8 *)&((ChannelControlRow36 *)(t0_page-0x6F8))-> age02 );
                    }
                    );
                    ties=1;
                }
                else if(signed_priority==cutoff) {
                    raw_priority=(u16)best_gate;
                    gate= ( {
                    a2_page=(u8 *)0x80110000U;
                    a2_page+=offset;
                        *( u16 *)((u8 *)&((ChannelControlRow36 *)(a2_page-0x6F8))-> value06 );
                    }
                    );
                    ties++;
                    if((u16)gate<raw_priority) {
                        best_age= ( {
                    t0_page=(u8 *)0x80110000U;
                    t0_page+=offset;
                            *( u16 *)((u8 *)&((ChannelControlRow36 *)(t0_page-0x6F8))-> age02 );
                        }
                        );
                        best_gate=gate;
                        best=index;
                    }
                    else if((u16)gate==raw_priority) {
                        /* ASM_BARRIER: retain the scheduling boundary between field capture and the next comparison. */
                        __asm__("" : :);
                        signed_age= ( {
                            u8 *page=(u8 *)0x80110000U;
                            /* ASM_BARRIER: capture the PAL row page before applying its index and field offset. */
                            __asm__("" : "+r"(page));
                            page+=( offset );
                            *( s16 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> age02 );
                        }
                        );
                        /* ASM_BARRIER: retain the scheduling boundary between field capture and the next comparison. */
                        __asm__("" : :);
                        age= ( {
                    at_page=(u8 *)0x80110000U;
                    at_page+=offset;
                            *( u16 *)((u8 *)&((ChannelControlRow36 *)(at_page-0x6F8))-> age02 );
                        }
                        );
                        if((s32)best_age<signed_age) {
                            best_age=age;
                            best=index;
                        }
                    }
                }
            }
            index++;
            loop_test=(u8)index;
            /* ASM_BARRIER: keep the byte loop comparison separate from the next byte index. */
            __asm__("" : "+r"(index));
            ix=(u8)index;
        }
        while(loop_test<limit);
    }
    if((u8)chosen==99) {
        ties_test=(u8)ties;
        chosen=best;
        if(!ties_test)chosen=(u8)spuVmMaxVoice;
    }
    count=spuVmMaxVoice;
    if((u8)chosen<count) {
        index=0;
        if(count>0) {
            u32 one2;
            u32 mask;
            s32 count2;
            /* ASM_BARRIER: keep the byte loop comparison separate from the next byte index. */
            __asm__("" : "+r"(index));
            one2=1;
            mask=_snd_vmask;
            count2=count;
            ix=(u8)index;
            do {
                if(!(mask&(one2<<ix))) {
                    /* ASM_BARRIER: retain the captured selection accumulator in its original temporary. */
                    register s32 off asm("$2")=ix*sizeof(ChannelControlRow36);
                    u32 v= ( {
                        u8 *page=(u8 *)0x80110000U;
                        /* ASM_BARRIER: capture the PAL row page before applying its index and field offset. */
                        __asm__("" : "+r"(page));
                        page+=( off );
                        *( u16 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> age02 );
                    }
                    );
                    v++;
                    do {
                    at_page=(u8 *)0x80110000U;
                    at_page+=off;
                        *( u16 *)((u8 *)&((ChannelControlRow36 *)(at_page-0x6F8))-> age02 )=( v );
                    }
                    while(0);
                }
                index++;
                loop_test=(u8)index;
                /* ASM_BARRIER: keep the byte loop comparison separate from the next byte index. */
                __asm__("" : "+r"(index));
                ix=(u8)index;
            }
            while(loop_test<count2);
        }
        offset=(u8)chosen*sizeof(ChannelControlRow36);
        do {
                    at_page=(u8 *)0x80110000U;
                    at_page+=offset;
            *( u16 *)((u8 *)&((ChannelControlRow36 *)(at_page-0x6F8))-> age02 )=( 0 );
        }
        while(0);
        initial=D_8011010F;
        do {
                    at_page=(u8 *)0x80110000U;
                    at_page+=offset;
            *( u16 *)((u8 *)&((ChannelControlRow36 *)(at_page-0x6F8))-> field2A )=( 0 );
        }
        while(0);
        do {
                    at_page=(u8 *)0x80110000U;
                    at_page+=offset;
            *( u16 *)((u8 *)&((ChannelControlRow36 *)(at_page-0x6F8))-> field1E )=( 0 );
        }
        while(0);
        initial=(s8)initial;
        do {
                    at_page=(u8 *)0x80110000U;
                    at_page+=offset;
            *( s16 *)((u8 *)&((ChannelControlRow36 *)(at_page-0x6F8))-> priority1A )=( initial );
        }
        while(0);
    }
    return (u8)chosen;
}

/* PAL scalar state is addressed from page 0x80110000. Pointer-valued bank
 * and tone slots remain full native pointers in the shared declarations. */
void _SsVmDoAllocate(void) {
    s32 i=0;
    u16 *initial=&_svm_voice_cursor;
    u16 *selected;
    u32 *mask;
    u32 one;
    u32 scaled;
    /* ASM_BARRIER: reuse V1 for the selected voice only after each preceding value dies. */
    register s32 voice asm("$3");
    /* ASM_BARRIER: materialize each PAL page in AT after preceding values have been consumed. */
    register u8 *store asm("$1");
    s16 program;
    s16 *selection;
    s32 at;
    /* ASM_BARRIER: reuse V0 for the selected value, flag loads and clamp comparison. */
    register u32 value asm("$2");
    s32 offset;
    /* ASM_BARRIER: retain the byte selection base in A3 through the final voice reload. */
    register s8 *tone_selection asm("$7");
    s32 toneIndex;
    u32 flags;
    s32 low;
    u16 *base;
    s32 offset2;
    u8 *load;
    /* ASM_BARRIER: capture the selector pointer before copying to selected. */
    __asm__("" : "+r"(initial));
    selected=initial;
    /* ASM_BARRIER: schedule selector copy before initializing shift unit. */
    __asm__("");
    one=1;
voice=*selected;
    mask=_svm_envx_hist;
    scaled=voice*8U;
    voice=(s16)voice;
    value=voice*sizeof(ChannelControlRow36);
    voice=32767;
store=(u8 *)0x80110000U;
store+=value;
    *(u16 *)(store - 0x6F8 + RR_FIELD_OFFSET(ChannelControlRow36, value06))=voice;
    do {
        i++;
        *mask &= ~(one << (s16)*selected);
        mask++;
    } while (i < 16);
    selection=D_80110116;
    program=selection[0];
    if ((s16)((u16)program & 1) > 0) {
        at=(program-1)/2;
        value=((u16 *)&_svm_pg[at][VAB_PROGRAM_SAMPLE_ADDRESSES])[0];
        offset=(scaled<<16);
    } else {
        at=((u32)(u16)program<<16);
        offset=(scaled<<16);
        at=((at>>16)-1)/2;
        value=((u16 *)&_svm_pg[at][VAB_PROGRAM_SAMPLE_ADDRESSES])[1];
    }
    offset>>=15;
    store=(u8 *)0x80110000U;
store+=offset;
    *(u16 *)(store - 0x1E8 + RR_FIELD_OFFSET(SpuVolumeCache, voices) + RR_FIELD_OFFSET(SpuCachedVolumeEntry, program06))=value;
    voice=selection[1];
load=(u8 *)0x80110000U;
load+=voice;
    value=load[-0x1E8]|8;
store=(u8 *)0x80110000U;
store+=voice;
    store[-0x1E8]=value;
    /* ASM_BARRIER: recapture the voice offset after publishing the first dirty flag. */
    __asm__("" : "+r"(scaled) : : "$7", "memory");
    tone_selection=D_80110107;
    offset=(scaled<<16);
    offset2=offset>>15;
    toneIndex=tone_selection[0]*16+tone_selection[5];
    voice=*(u16 *)&_svm_tn[toneIndex][VAB_TONE_ADSR1];
    /* ASM_BARRIER: schedule tone value10 before forming the cache destination base. */
    __asm__("");
    base=&_svm_sreg_dirty.voices[0].tone08;
    store=(u8 *)0x80110000U;
store+=offset2;
    *(u16 *)(store - 0x1E8 + RR_FIELD_OFFSET(SpuVolumeCache, voices) + RR_FIELD_OFFSET(SpuCachedVolumeEntry, tone08))=voice;
    flags=*(u16 *)&_svm_tn[toneIndex][VAB_TONE_ADSR2];
    value=_svm_damper;
    value+=(flags&31);
    low=value;
    /* ASM_BARRIER: retain the full sum separately from its signed low16 comparison. */
    __asm__("" : "+r"(value));
    flags&=0xFFE0;
    if ((s16)value >= 32) low=31;
    voice=low|flags;
    /* ASM_BARRIER: retain the cache base before the final indexed halfword store. */
    __asm__("" : "+r"(base));
    load=(u8 *)((unsigned long)base+offset2);
    ((u16 *)load)[1]=voice;
    voice=((s16 *)(tone_selection+1))[8];
load=(u8 *)0x80110000U;
load+=voice;
    value=load[-0x1E8]|0x30;
store=(u8 *)0x80110000U;
store+=voice;
    store[-0x1E8]=value;
}
