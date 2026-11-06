#include "rr/overlay662_setup.h"

void ovl662_func_80116020(void)
{
    s32 i;
    D_800F2DE4=0;
    ScreenFrame=0;
    ovl662_D_801272D4=0;
    ovl662_D_801272C0=0;
    D_800AD520=2;
    D_800F4D98.bytes[0]=2;
    D_800F3826=0;
    for(i=0;i<4;i++)func_8003E040(i,-2,0,1);
    func_8003DC9C(D_800F4D8C->glyph07,D_800F4D8C->x08,D_800F4D8C->y0A,D_800F4D8C->w0C,D_800F4D8C->h0E,0,0);
    func_8003E928(0,12,72,240,32,0,0,4,2,D_800AD520);
    func_8003D228(0,36,336,248,34,0,0x301);
    D_800F3AFC->half02=1;
}
