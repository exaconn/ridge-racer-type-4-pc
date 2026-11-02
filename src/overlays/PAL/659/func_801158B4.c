#include "standings.h"

void ovl659_func_801158B4(void)
{
    s32 values[6];
    u16 indices[6];
    s32 n, rank;
    s32 *destination, *source;
    u16 *order;
    /* ASM_BARRIER: retain the original shared outer-loop counter register. */
    register s32 i __asm__("$9");
    s32 j, previous;
    s32 *left, *right;
    s32 l, r;
    u16 *slot, *prior;
    u16 index;
    u8 *vehicle;
    n = 6;
    i = 0;
    destination = values; source = D_800F4C80; order = indices;
    for (; i < 6; destination++, source++, i++, order++) {
        *order = i;
        *destination = *source;
        if (*destination < 0) { n = i; break; }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = n - 1; j > i; j = previous) {
            previous = j - 1;
            left = &values[previous];
            right = &values[j];
            l = *left;
            r = *right;
            if (r < l) {
                slot = &indices[j];
                index = *slot;
                *right = l;
                prior = &indices[previous];
                *left = r;
                *slot = *prior;
                *prior = index;
            }
        }
    }
    rank = D_800F4C64;
    if (rank == -1) {
        vehicle = (u8 *)D_800ABEF8;
        /* ASM_BARRIER: retain the complete vehicle base for both halfword accesses. */
        __asm__("" : "+r"(vehicle));
        (*(s16 *)(vehicle + 0x2A8)) = rank;
        if ((*(s16 *)(vehicle + 0x2AA)) > 0)
            D_800F4C80[(*(s16 *)(vehicle + 0x2AA)) - 1] = rank;
    } else {
        D_800AC1A0 = indices[0];
    }
}
