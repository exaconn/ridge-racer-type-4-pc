/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
#include "rr/channel_stream.h"
#include "psyq/libsnd/sstick.h"

void _SsSeqPlay(s32 bank, s32 row);
void _SsSeqGetEof(s32 bankInput,s32 rowInput);

void _SsSeqPlay(s32 bank, s32 row) {
    u32 bank_bits = (u32)bank << 16;
    /* ASM_BARRIER: Retain the original register across record lookup and counter updates. */
    register u32 row_bits __asm__("$5");
    /* ASM_BARRIER: Retain the original register across record lookup and counter updates. */
    register s32 index __asm__("$4");
    /* ASM_BARRIER: Retain the original register across record lookup and counter updates. */
    register RrLibStateB0 **table __asm__("$3");
    RrLibStateB0 *state;
    s32 units;
    s32 period;
    s16 delay;
    s32 current, delta;
    u32 accumulated;
    s32 result;
    register u32 reset_value;
    index = (s16)(bank_bits >> 16) * (s32)sizeof(*table);
    /* ASM_BARRIER: Complete the bank index before forming the preserved row bits. */
    __asm__("" : "=r"(row) : "0"(row));
    row_bits = (u32)row << 16;
    units = (s16)(row_bits >> 16) * 11;
    /* ASM_BARRIER: Complete both index calculations before materializing the PAL lookup page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$3");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    state = (RrLibStateB0 *)((u8 *)table[-0xF98 / 4] + units * 16);
    /* h48[5] is the delay at 0x52; h48[6] is the period at 0x54. */
    period = state->h48[6];
    current = state->w90;
    /* ASM_BARRIER: Load the current counter before the unsigned period snapshot. */
    __asm__ volatile("" : : "r"(current) : "memory");
    reset_value = (u16)state->h48[6];
    /* ASM_BARRIER: Keep the unsigned reload value available before the fast-path branches. */
    __asm__("" : "=r"(reset_value) : "0"(reset_value));
    delta = (s32)((u32)current - (u32)period);
    if (delta > 0) {
        delay = state->h48[5];
        if (delay > 0) {
            state->h48[5] = (u16)state->h48[5] - 1;
            return;
        }
        if (delay == 0) {
            state->h48[5] = reset_value;
            result = (s32)((u32)state->w90 - 1);
            goto finish;
        }
        state->w90 = delta;
        return;
    }
    if (period >= current) {
        register s32 saved_bank;
        register s32 saved_row;
        register s32 update;
        register s32 next_period;
        accumulated = (u32)current;
        saved_bank = (s32)bank_bits;
        saved_row = (s32)row_bits;
        do {
again:
            _SsGetSeqData(saved_bank >> 16, saved_row >> 16);
            update = state->w90;
            if (update == 0) goto again;
            next_period = state->h48[6];
            accumulated += (u32)update;
        } while ((s32)accumulated < next_period);
        result = (s32)(accumulated - (u32)next_period);
finish:
        state->w90 = result;
    }
}

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
