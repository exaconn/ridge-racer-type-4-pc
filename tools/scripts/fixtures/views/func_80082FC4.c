/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses -fno-force-mem */

#include "rr/channels.h"

s32 _SsVmGetSeqVol(s32,u16 *,u16 *);

void _SsVmSetSeqVol(s32,u16,u16,s32);

void _SsSndCrescendo(s32 bankInput,s32 rowInput)
{
    s16 bank=bankInput,row=rowInput;
    u16 left,right;
    s32 bankBits=(s32)((u32)bank<<16);
    ChannelValueRecordB0 **table=_ss_score;
    /* Scale the signed bank offset by the native pointer-table stride. */
    ChannelValueRecordB0 **slot=(ChannelValueRecordB0 **)((u8*)table+(long)(bankBits>>14)*(long)(sizeof(*table)/sizeof(u32)));
    s32 offset=(s16)row*(s32)sizeof(ChannelValueRecordB0);
    ChannelValueRecordB0 *base=*slot;
    ChannelValueRecordB0 *record=(ChannelValueRecordB0 *)((u8 *)base+offset);
    register ChannelValueRecordB0 *current __asm__("$2");
    s32 elapsed=(s32)((u32)record->rampElapsedA0+1U);
    s32 duration=record->rampDuration9C;
    s32 delta;
    s32 l,r;
    s32 packed;
    record->rampElapsedA0=elapsed;
    if(duration<elapsed) {
        current=(ChannelValueRecordB0 *)((u8 *)*slot+offset);
        goto clear;
    } else {
        register s32 product __asm__("$2")=(s32)((u32)(s32)record->rampDelta48*(u32)elapsed);
        delta=product/duration;
        delta=(s32)((u32)delta-(u32)(s32)record->rampApplied4A);
        if(delta!=0) {
            packed=(s16)((u32)bank|((u32)row<<8));
            record->rampApplied4A=(u16)record->rampApplied4A+(u32)delta;
            _SsVmGetSeqVol(packed,&left,&right);
            l=(s32)((u32)left+(u32)delta);
            if(l>=128) l=127;
            if(l<0) l=0;
            r=(s32)((u32)right+(u32)delta);
            if(r>=128) r=127;
            if(r<0) r=0;
            _SsVmSetSeqVol(packed,(u16)l,(u16)r,1);
            if((l==127&&r==l)||(l==0&&r==0)) {
                current=&_ss_score[(s16)bank][(s16)row];
clear:
                current->mode98&=~0x10;
            }
        }
    }
    _SsVmGetSeqVol((s16)((u32)bank|((u32)row<<8)),&record->volume5C,&record->volume5E);
}
