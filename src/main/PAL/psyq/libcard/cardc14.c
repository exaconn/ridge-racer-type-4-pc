/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"
#include "psyq/libcard/cardc07.h"
#include "psyq/libcard/cardc161.h"
#include "psyq/libcard/cardc162.h"

extern int func_8009C87C(), func_8009C91C();

int func_8009D424();
int func_8009D4E0();
s32 func_8009D5B4(s32 request, u32 *first, u32 *second);

/* legacy typing of shared globals in this part of the unit */
#define D_80114718 (*(int (*))&D_80114718)
#define D_80114734 ((int *)D_80114734)
#define D_80114738 ((int *)D_80114738)

int func_8009D424(a0,a1) int a0; int a1; {
    int r;
    if (!(D_800ABDE4 & 1)) {
        r = 0;
    } else {
        r = 1;
        D_800ABDF4 = 0;
        D_800ABDF8 = 0;
        D_80114730 = 0;
        D_800ABDE8 = 0;
        D_800ABDEC = a0;
    }
    if (r == 0) return 0;
    D_80114724 = 0x6000300;
    D_80114728 = 4;
    D_80114870 = 0xA;
    D_80114734 = (int *)func_8009C87C;
    D_80114738 = (int *)func_8009C91C;
    D_80114718 = a1;
    D_800ABDE4 = 2;
    return 1;
}

#undef D_80114718
#undef D_80114734
#undef D_80114738

/* legacy typing of shared globals in this part of the unit */
#define D_8011472C (*(char (*))&D_8011472C)
#define D_80114718 ((char *)D_80114718)
#define D_80114734 ((int *)D_80114734)
#define D_80114738 ((int *)D_80114738)

int func_8009D4E0(a0, a1) int a0; int a1; {
    int r;
    char *p;
    if (!(D_800ABDE4 & 1)) {
        r = 0;
    } else {
        r = 1;
        D_800ABDF4 = 0;
        D_800ABDF8 = 0;
        D_80114730 = 0;
        D_800ABDE8 = 0;
        D_800ABDEC = a0;
    }
    if (r == 0) return 0;
    if (a1 != 0) {
        D_80114724 = 0xD800008;
    } else {
        D_80114724 = 0xD800004;
    }
    p = &D_8011472C;
    D_80114718 = p;
    *p = 2;
    D_80114728 = 1;
    D_80114870 = 9;
    D_80114734 = (int *)func_8009CA8C;
    D_80114738 = (int *)func_8009CB50;
    D_800ABDE4 = 2;
    return 1;
}

#undef D_8011472C
#undef D_80114718
#undef D_80114734
#undef D_80114738

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
