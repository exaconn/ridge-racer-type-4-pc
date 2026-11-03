/* CC1_FLAGS: -mno-split-addresses */
extern int func_8008B4E0(), func_800954D8(), func_800959B8();
#include "rr/gpu.h"
extern int D_800ABBE8, D_800ABBF8;
extern int D_800ABBEC;
extern char D_801127D0[], D_801128D8[];
int func_800950D4(a0)
int a0;
{
    int t;
    t = func_8008B4E0(0);
    D_800ABBEC = 0;
    D_800ABBF8 = t;
    D_800ABBE8 = D_800ABBEC;
    /* Ordinary register views retain the original reset scheduling. */
    switch (a0 & 7) {
    case 5:
    case 0:
        *(u32 *)D_800ABBD4 = 0x401;
        *(u32 *)D_800ABBE4 |= 0x800;
        *(u32 *)D_800ABBC8 = 0;
        func_800959B8(D_801127D0, 0, 0x100);
        func_800959B8(D_801128D8, 0, 0x1800);
        break;
    case 1:
    case 3:
        *(u32 *)D_800ABBD4 = 0x401;
        *(u32 *)D_800ABBE4 |= 0x800;
        *(u32 *)D_800ABBC8 = 0x02000000;
        *(u32 *)D_800ABBC8 = 0x01000000;
        break;
    }
    func_8008B4E0(D_800ABBF8);
    if ((a0 & 7) == 0) return func_800954D8(a0);
    return 0;
}
