/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"
#include "psyq/libcard/cardc09.h"
#include "psyq/libcard/cardc10.h"

s32 func_8009CB84(s32 request, s32 parameter, u8 *buffer, u8 *first) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 14;
    D_80114734 = func_8009CC44;
    D_80114738 = func_8009CCF0;
    active = 2;
    D_80114724 = parameter;
    D_80114718 = buffer;
    D_8011471C = first;
    D_80114728 = 0;
    D_8011472C.value = 0;
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
