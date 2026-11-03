/* GCC_VERSION: 2.7.2 */

#include "rr/channels.h"

extern void func_800897A4(s32,s32,s32,s32);

void func_800841E4(s16 bank, s32 left, s32 right) {
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
    /* PAL mapping: table[-0xF98 / 4] selects D_8010F068[bank]. */
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
        func_800897A4(bank, argument_left, (u16)argument_right, 1);
    }
}
