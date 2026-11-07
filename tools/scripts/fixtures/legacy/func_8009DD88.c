/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"

extern s32 _cardGetIdTx(void);

s32 _cardGetIdBegin(s32 request, u32 *first, u32 *second) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    CardCurrentCommand = 101;
    CardCommandReader = _cardGetIdTx;
    CardWriteHandler = _cardGetIdRx;
    active = 2;
    D_8011471C = first;
    D_80114720 = second;
    {
        /* ASM_BARRIER: preserve the original assembler temporary for the mapped store. */
        register s32 *base __asm__("$1");
        /* ASM_BARRIER: finish parameter publication before materializing the state address. */
        __asm__ volatile("" : : : "$1", "memory");
        /* PAL RAM mapping: base[-0x421C / 4] is D_800ABDE4. */
        base = (s32 *)0x800B0000U;
        /* ASM_BARRIER: expose the active-state store in the branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[-0x421C / 4] = active;
    }
    return result;
}
