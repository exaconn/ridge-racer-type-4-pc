#include "rr/overlay662_garage.h"
/* Overlay-local storage; the same RAM can have another meaning in another image. */
void ovl662_func_8011E7C0(s32 a, s32 b, s32 c, s32 d)
{
    ovl662_D_80127668.halves[24] = a;
    ovl662_D_80127668.halves[25] = b;
    ovl662_D_80127668.halves[26] = c;
    ovl662_D_80127668.halves[27] = d;
}
