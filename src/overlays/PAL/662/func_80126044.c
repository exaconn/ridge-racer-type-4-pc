#include "rr/overlay662_tim.h"

void ovl662_func_80126044(void) {
    u32 *p;
    GarageTimBytes *src2C = ovl662_D_801276A8.tim2C[ovl662_D_801276A8.active21];
    u32 *img38 = ovl662_D_801276A8.image38[ovl662_D_801276A8.active21];
    PaletteBytes32 *pal48 = (PaletteBytes32 *)(Palette16 *)ovl662_D_801276A8.palette48[ovl662_D_801276A8.active21];

    p = D_800F3B28.words;
    *(GarageTimBytes *)p = *src2C;
    *p++ = 0x10;
    *p++ = 8;
    *p++ = 44;
    *p++ = 0x00400280;
    *p++ = 0x00010010;
    *(PaletteBytes32 *)p = *pal48;
    p += 8;
    *p++ = 0x80C;
    *p++ = 0x280;
    *p++ = 0x00400010;
    ovl662_func_80125F60(img38, p);
}
