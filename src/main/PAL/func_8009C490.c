/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"
extern s32 func_8009C55C(void), func_8009C610(s32);

s32 func_8009C490(s32 request, s32 parameter, u8 *buffer) {
    s32 mode;
    if (parameter >= 0) {
        if (parameter >= 16) return 0;
        {
            /* ASM_BARRIER: Use the original assembler temporary for parameter publication. */
            register s32 *base __asm__("$1");
            /* ASM_BARRIER: Keep parameter validation before materializing its PAL address. */
            __asm__ volatile("" : : : "$1", "memory");
            /* PAL mapping: base[0x4724 / 4] is D_80114724. */
            base = (s32 *)0x80110000U;
            /* ASM_BARRIER: Expose the PAL parameter store in the branch delay slot. */
            __asm__("" : "=r"(base) : "0"(base));
            base[0x4724 / 4] = parameter;
        }
    } else D_80114724 = -1;
    if (!rr_command_begin(request)) return 0;
    mode = 2;
    /* ASM_BARRIER: Form command state 2 before callback materialization and parameter stores. */
    __asm__ volatile("" : "=r"(mode) : "0"(mode) : "$4", "memory");
    D_80114734 = func_8009C55C;
    D_8011472C.buffer = buffer;
    D_80114718 = buffer;
    D_80114870 = mode;
    D_80114738 = func_8009C610;
    D_800ABDE4 = mode;
    return 1;
}
