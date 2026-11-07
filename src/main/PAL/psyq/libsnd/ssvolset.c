/* GCC_VERSION: 2.7.2 */
#include "rr/channels.h"
#include "rr/library.h"
#include "psyq/libsnd/vmchval.h"
#include "psyq/libsnd/ssvolset.h"
#include "psyq/libsnd/ssatr2.h"

void _SsSndSetVol(s32 bank,s32 row,s32 left,s32 right);
void SsSeqSetVol(s16 bank, s32 left, s32 right);
void SsSepSetVol(s32 bank, s32 row, s32 left, s32 right);
void SsSeqGetVol(int a, int b, int c, int d);

void _SsSndSetVol(s32 bank,s32 row,s32 left,s32 right) {
    register u32 raw_bank=bank;
    /* ASM_BARRIER: Retain the original register for lookup, argument preparation, or return scheduling. */
    register s32 left_value __asm__("$9")=left;
    /* ASM_BARRIER: Retain the original register for lookup, argument preparation, or return scheduling. */
    register s32 right_value __asm__("$10");
    /* ASM_BARRIER: Retain the original register for lookup, argument preparation, or return scheduling. */
    register RrChannelValueRecordB0 **table __asm__("$3");
    register s32 index;
    s32 units;
    RrChannelValueRecordB0 *state,*base;
    /* ASM_BARRIER: Form the packed channel in its original callback argument register. */
    /* ASM_BARRIER: Retain the original register for lookup, argument preparation, or return scheduling. */
    register u32 encoded __asm__("$4");
    index=(s16)raw_bank*sizeof(*table);
    units=(s16)row*11;
    /* ASM_BARRIER: Finish both indices before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index),"r"(units) : "$3");
    table=(RrChannelValueRecordB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table=(RrChannelValueRecordB0 **)((u8 *)table+index);
    base=table[-0xF98/4];
    /* ASM_BARRIER: Load the bank base before finishing the row byte offset. */
    __asm__("" : "=r"(base) : "0"(base));
    state=(RrChannelValueRecordB0 *)((u8 *)base+units*16);
    right_value=right;
    if(state->mode98!=1) {
        state->value58=left;
        state->value5A=right;
    } else {
        encoded=(u32)row<<8;
        encoded=raw_bank|encoded;
        encoded<<=16;
        _SsVmSetSeqVol((s32)encoded>>16,(u16)left_value,(u16)right_value,1);
    }
}

void SsSeqSetVol(s16 bank, s32 left, s32 right) {
    register s32 right_value = right;
    /* ASM_BARRIER: Retain the original register across the bank lookup and mode branch. */
    register s32 left_value __asm__("$9")= left;
    /* ASM_BARRIER: Retain the original register across the bank lookup and mode branch. */
    register RrChannelValueRecordB0 **table __asm__("$7");
    s32 index;
    register RrChannelValueRecordB0 *state;
    /* ASM_BARRIER: Retain the original register across the bank lookup and mode branch. */
    register s32 argument_right __asm__("$6");
    /* ASM_BARRIER: Preserve the right value before reusing its incoming argument register. */
    __asm__("" : "=r"(bank):"0"(bank),"r"(right_value):"$6");
    bank = (s16)bank;
    index = bank * sizeof(*table);
    /* ASM_BARRIER: Scale the bank index before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index):"$7");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (RrChannelValueRecordB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without a symbol-address macro. */
    __asm__("" : "=r"(table):"0"(table));
    table = (RrChannelValueRecordB0 **)((u8 *)table+index);
    state = table[-0xF98 / 4];
    argument_right = right_value;
    if (state->mode98 != 1) {
        state->value58= left;
        state->value5A = right_value;
    } else {
        register s32 argument_left = (u16)left_value;
        /* ASM_BARRIER: Normalize the left argument before the right argument and final call constant. */
        __asm__("" : "=r"(argument_right):"0"(argument_right),"r"(argument_left):"$7");
        _SsVmSetSeqVol(bank, argument_left, (u16)argument_right, 1);
    }
}

void SsSepSetVol(s32 bank, s32 row, s32 left, s32 right) {
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register s32 raw_bank __asm__("$8") = bank;
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register s32 left_value __asm__("$9") = left;
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register s32 right_value __asm__("$10");
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register RrLibStateB0 **table __asm__("$3");
    register RrLibStateB0 *bank_base;
    s32 index;
    s32 units;
    RrLibStateB0 *state;
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register u32 row_bits __asm__("$4");
    /* ASM_BARRIER: Retain the left value before reusing its incoming argument register for lookup. */
    __asm__("" : "=r"(left_value) : "0"(left_value) : "$4");
    index = (s16)bank * sizeof(*table);
    units = (s16)row * 11;
    /* ASM_BARRIER: Complete index arithmetic before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$3");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    bank_base = table[-0xF98 / 4];
    /* ASM_BARRIER: Load the bank base before forming the final record address. */
    __asm__("" : "=r"(bank_base) : "0"(bank_base));
    state = bank_base + (s16)row;
    right_value = right;
    if (state->w98 != 1) {
        state->h58[0] = left;
        state->h58[1] = right;
        return;
    }
    row_bits = (u32)row << 8;
    _SsVmSetSeqVol((s16)(raw_bank | row_bits), left_value & 0xFFFF, right_value & 0xFFFF, 1);
}

void SsSeqGetVol(int a, int b, int c, int d) { _SsVmGetSeqVol((short)(a | (b << 8)), c, d); }
