#include "rr/standings.h"
void ovl660_func_801162D8(void)
{
    s32 values[6];
    u16 indices[6];
    s32 n;
    s32 *destination, *source;
    u16 *order;
    s32 i;
    s32 j, previous;
    s32 *left, *right;
    s32 l, r;
    u16 *slot, *prior;
    u16 index;
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
    D_800AC1A0 = indices[0];
}
