/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses -fno-force-mem */

#include "rr/channels.h"

s32 func_80089D28(s32,u16 *,u16 *);

void func_800897A4(s32,u16,u16,s32);

void func_80082FC4(s32 bankInput,s32 rowInput)
{
    s16 bank=bankInput,row=rowInput;
    u16 left,right;
    s32 bankBits=(s32)((u32)bank<<16);
    RrChannelValueRecordB0 **table=D_8010F068;
    /* Scale the signed bank offset by the native pointer-table stride. */
    RrChannelValueRecordB0 **slot=(RrChannelValueRecordB0 **)((u8*)table+(long)(bankBits>>14)*(long)(sizeof(*table)/sizeof(u32)));
    s32 offset=(s16)row*(s32)sizeof(RrChannelValueRecordB0);
    RrChannelValueRecordB0 *base=*slot;
    RrChannelValueRecordB0 *record=(RrChannelValueRecordB0 *)((u8 *)base+offset);
    register RrChannelValueRecordB0 *current __asm__("$2");
    s32 elapsed=(s32)((u32)record->rampElapsedA0+1U);
    s32 duration=record->rampDuration9C;
    s32 delta;
    s32 l,r;
    s32 packed;
    record->rampElapsedA0=elapsed;
    if(duration<elapsed) {
        current=(RrChannelValueRecordB0 *)((u8 *)*slot+offset);
        goto clear;
    } else {
        register s32 product __asm__("$2")=(s32)((u32)(s32)record->rampDelta48*(u32)elapsed);
        delta=product/duration;
        delta=(s32)((u32)delta-(u32)(s32)record->rampApplied4A);
        if(delta!=0) {
            packed=(s16)((u32)bank|((u32)row<<8));
            record->rampApplied4A=(u16)record->rampApplied4A+(u32)delta;
            func_80089D28(packed,&left,&right);
            l=(s32)((u32)left+(u32)delta);
            if(l>=128) l=127;
            if(l<0) l=0;
            r=(s32)((u32)right+(u32)delta);
            if(r>=128) r=127;
            if(r<0) r=0;
            func_800897A4(packed,(u16)l,(u16)r,1);
            if((l==127&&r==l)||(l==0&&r==0)) {
                current=&D_8010F068[(s16)bank][(s16)row];
clear:
                current->mode98&=~0x10;
            }
        }
    }
    func_80089D28((s16)((u32)bank|((u32)row<<8)),&record->volume5C,&record->volume5E);
}
