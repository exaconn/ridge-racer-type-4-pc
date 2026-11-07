/* CC1_FLAGS: -mno-split-addresses */

#include "rr/channel_stream.h"

void _SsSeqGetEof(s32 bankInput,s32 rowInput) {
    s16 bank=bankInput,row=rowInput;
    s32 bits=(s32)((u32)bank<<16);
    RrChannelStreamB0 **table=_ss_score;
    /* Keep table indexing proportional to the native pointer width. */
    RrChannelStreamB0 **slot=(RrChannelStreamB0 **)((u8*)table+(long)(bits>>14)*(long)(sizeof(*table)/sizeof(u32)));
    RrChannelStreamB0 *record=&(*slot)[row];
    s32 elapsed=record->elapsed21+1;
    s32 duration=record->duration20;
    record->elapsed21=elapsed;
    if(duration==0) {
        record->accumulator88=0;
        record->state1C=0;
        record->accumulator90=0;
        if((*slot)[row].mode98&0x400)record->cursor00=record->alternate0C;
        else record->cursor00=record->start04;
    } else if((s8)elapsed<duration) {
        record->accumulator88=0;
        record->state1C=0;
        record->accumulator90=0;
        if((*slot)[row].mode98&0x400) {
            u8 *cursor=record->alternate0C;
            u8 *saved=*(u8 *volatile *)&record->alternate0C;
            record->cursor00=cursor;
            record->saved08=saved;
        } else {
            u8 *cursor=record->start04;
            u8 *saved=*(u8 *volatile *)&record->start04;
            record->cursor00=cursor;
            record->saved08=saved;
        }
    } else {
        (*slot)[row].mode98&=~1;
        (*slot)[row].mode98&=~8;
        (*slot)[row].mode98&=~2;
        (*slot)[row].mode98|=0x200;
        (*slot)[row].mode98|=4;
        record->active14=0;
        if((*slot)[row].mode98&0x400)record->saved08=record->alternate0C;
        else record->saved08=record->start04;
        if(record->restartBank22!=-1) {
            /* Preserve the original repeated signed-byte read before restart. */
            __asm__("" : : : "memory");
            record->active14=0;
            _SsSndNextSep(record->restartBank22,record->restartRow23);
            _SsVmSeqKeyOff((s16)((u32)bank|((u32)row<<8)));
        }
        _SsVmSeqKeyOff((s16)((u32)bank|((u32)row<<8)));
        record->accumulator90=record->pitch54;
    }
}
