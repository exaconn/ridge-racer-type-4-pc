/* GCC_VERSION: 2.7.2 */

#include "rr/channels.h"

extern void _SsVmSetSeqVol(s32,s32,s32,s32);

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
