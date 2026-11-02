/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses -fno-force-mem */
#include "rr/channel_stream.h"
void func_80084314(s32 bankInput,s32 rowInput) {
    s16 row=rowInput;
    s32 savedBank;
    s32 rawBank=bankInput;
    s32 bits=(s32)((u32)rawBank<<16);
    RrChannelStreamB0 *(*table)[]=&D_8010F068;
    RrChannelStreamB0 **slot=(RrChannelStreamB0 **)((u8*)table+(long)(bits>>14)*(long)(sizeof(**table)/sizeof(u32)));
    RrChannelStreamB0 *base=*slot;
    RrChannelStreamB0 *record=({
        /* Retain the initialized bank argument until the table root is ready. */
        __asm__("" : "+r"(rawBank) : "r"(base));
        savedBank=rawBank;
        &base[row];
    });
    register RrChannelStreamB0 *current __asm__("$3");
    s32 remaining=(s32)((u32)record->tempoRemainingA8-1U);
    s32 interval;
    u32 value,pitch;
    u32 nextValue;
    s32 crossed;
    record->tempoRemainingA8=remaining;
    if(remaining<0) {
        (*slot)[row].mode98&=~0x40;
        current=&(*slot)[row];
        goto clear;
    }
    interval=record->interval4E;
    if(interval>0) {
        u32 target;
        if(remaining%interval!=0)return;
        value=record->tempo94;
        target=record->targetTempoAC;
        if(target<value)nextValue=value-1;
        else if((crossed=value<target))nextValue=value+1;
        else goto adjusted;
        record->tempo94=nextValue;
    } else {
        u32 target;
        value=record->tempo94;
        target=record->targetTempoAC;
        if(target<value) {
            nextValue=value+(u32)interval;
            record->tempo94=nextValue;
            crossed=nextValue<target;
        } else {
            if(!(crossed=value<target))goto adjusted;
            nextValue=value-(u32)interval;
            /* Preserve the target reload between computing and storing tempo. */
            __asm__("" : : : "memory");
            target=record->targetTempoAC;
            record->tempo94=nextValue;
            crossed=target<nextValue;
        }
        if(crossed)record->tempo94=target;
    }
adjusted:
    pitch=(u32)(s32)record->scale50*record->tempo94;
    pitch=pitch*10U/(D_8010F064*60U);
    record->pitch54=(s16)pitch;
    if((s16)pitch<=0)record->pitch54=1;
    if(record->tempoRemainingA8==0 || record->tempo94==record->targetTempoAC) {
        s32 endBits=(s32)((u32)savedBank<<16);
        RrChannelStreamB0 *(*endTable)[]=&D_8010F068;
        RrChannelStreamB0 **endSlot=(RrChannelStreamB0 **)((u8*)endTable+(long)(endBits>>14)*(long)(sizeof(**endTable)/sizeof(u32)));
        (*endSlot)[row].mode98&=~0x40;
        current=&(*endSlot)[row];
clear:
        current->mode98&=~0x80;
    }
}
