#include "rr/channel_voice_choice.h"
#include "rr/spu_voice_cache.h"

s32 _SsVmAlloc(void) {
    u32 chosen;
    u32 best_gate;
    s32 ties;
    u32 best_age;
    u32 best;
    u32 index;
    s32 priority;
    s32 limit;
    u32 reserved;
    u32 one;
    s32 offset;
    s32 signed_priority;
    u32 raw_priority;
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
    u8 *at_page;
    u8 *a2_page;
    chosen=99;
    best_gate=65535;
    ties=0;
    best_age=0;
    best=99;
    index=0;
    initial=D_8011010F;
    count=spuVmMaxVoice;
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
                    page+=( offset );
                    *( s8 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> flags1D );
                }
                ) ==0 && ( {
                    u8 *page=(u8 *)0x80110000U;
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
                raw_priority = 0x80110000U;
                raw_priority += offset;
                raw_priority = *( u16 *)((u8 *)&((ChannelControlRow36 *)((u8 *)raw_priority-0x6F8))-> priority1A );
                if(signed_priority<cutoff) {
                    priority=raw_priority;
                    best=index;
                    best_gate = 0x80110000U;
                    best_gate += offset;
                    best_gate = *( u16 *)((u8 *)&((ChannelControlRow36 *)((u8 *)best_gate-0x6F8))-> value06 );
                    best_age = 0x80110000U;
                    best_age += offset;
                    best_age = *( u16 *)((u8 *)&((ChannelControlRow36 *)((u8 *)best_age-0x6F8))-> age02 );
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
                        best_age = 0x80110000U;
                        best_age += offset;
                        best_age = *( u16 *)((u8 *)&((ChannelControlRow36 *)((u8 *)best_age-0x6F8))-> age02 );
                        best_gate=gate;
                        best=index;
                    }
                    else if((u16)gate==raw_priority) {
                        signed_age= ( {
                            u8 *page=(u8 *)0x80110000U;
                            page+=( offset );
                            *( s16 *)((u8 *)&((ChannelControlRow36 *)(page-0x6F8))-> age02 );
                        }
                        );
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
            one2=1;
            mask=_snd_vmask;
            count2=count;
            ix=(u8)index;
            do {
                if(!(mask&(one2<<ix))) {
                    s32 off=ix*sizeof(ChannelControlRow36);
                    u32 v= ( {
                        u8 *page=(u8 *)0x80110000U;
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
    s32 voice;
    u8 *store;
    s16 program;
    s16 *selection;
    s32 at;
    u32 value;
    s32 offset;
    s8 *tone_selection;
    s32 toneIndex;
    u32 flags;
    s32 low;
    u16 *base;
    s32 offset2;
    u8 *load;
    selected=initial;
    one=1;
voice=*selected;
    mask=_svm_envx_hist;
    scaled=voice*8U;
    voice<<=16;
    voice>>=16;
    value=voice*sizeof(ChannelControlRow36);
    voice=32767;
store=(u8 *)0x80110000U;
store+=value;
    ((ChannelControlRow36 *)(store - 0x6F8))->value06=voice;
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
    ((SpuVolumeCache *)(store - 0x1E8))->voices[0].program06=value;
    voice=selection[1];
load=(u8 *)0x80110000U;
load+=voice;
    value=load[-0x1E8]|8;
store=(u8 *)0x80110000U;
store+=voice;
    store[-0x1E8]=value;
    tone_selection=D_80110107;
    offset=(scaled<<16);
    offset2=offset>>15;
    toneIndex=tone_selection[0]*16+tone_selection[5];
    voice=*(u16 *)&_svm_tn[toneIndex][VAB_TONE_ADSR1];
    base=&_svm_sreg_dirty.voices[0].tone08;
    store=(u8 *)0x80110000U;
store+=offset2;
    ((SpuVolumeCache *)(store - 0x1E8))->voices[0].tone08=voice;
    flags=*(u16 *)&_svm_tn[toneIndex][VAB_TONE_ADSR2];
    value=_svm_damper;
    value+=(flags&31);
    low=value;
    flags&=0xFFE0;
    if ((s16)value >= 32) low=31;
    voice=low|flags;
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
