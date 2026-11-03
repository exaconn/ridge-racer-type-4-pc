/* GCC_VERSION: 2.7.2 */

#include "rr/commands.h"

/* legacy typing of shared globals in this part of the unit */
#define D_80114718 (*(int (*))&D_80114718)

#define D_80114734 ((int *)D_80114734)

#define D_80114738 ((int *)D_80114738)

#undef D_80114718

#undef D_80114734

#undef D_80114738

/* legacy typing of shared globals in this part of the unit */
#define D_8011472C (*(char (*))&D_8011472C)

#define D_80114718 ((char *)D_80114718)

#define D_80114734 ((int *)D_80114734)

#define D_80114738 ((int *)D_80114738)

#undef D_8011472C

#undef D_80114718

#undef D_80114734

#undef D_80114738

extern s32 func_8009D65C(void);

extern s32 func_8009D680(s32);

s32 func_8009D5B4(s32 request, u32 *first, u32 *second) {
    s32 result;
    s32 active;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 100;
    D_80114734 = func_8009D65C;
    D_80114738 = func_8009D680;
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
