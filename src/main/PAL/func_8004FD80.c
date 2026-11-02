#include "types.h"
extern int func_800534D8();extern int func_80053D24();extern int func_8004FCB4();extern int func_80052548();extern int func_800501BC();extern int func_80050EC4();extern int func_80051378();extern int func_80051554();extern int func_8004F418();extern void func_800518E0(void);
extern struct { char p0[0x286C]; s16 x286C; s16 x286E; s16 x2870; char p1[0x2]; s32 x2874; s32 x2878; } D_800F4DB8;
void func_8004FD80(void) {
    int i;
    u32 *p;
    int a;
    if (D_800F4DB8.x2878 != D_800F4DB8.x2874) {
        a = D_800F4DB8.x2874 - 1;
        D_800F4DB8.x286C = 1;
        if ((a & 1) == 0) D_800F4DB8.x286E = 1;
        if ((a & 3) == 0) D_800F4DB8.x2870 = 1;
    } else {
        D_800F4DB8.x286C = 0;
        D_800F4DB8.x286E = 0;
        D_800F4DB8.x2870 = 0;
    }
    i = 0;
    D_800F4DB8.x2878 = D_800F4DB8.x2874;
    func_800534D8();
    func_80053D24();
    func_8004FCB4();
    func_80052548();
    for (p = (u32 *)((char *)&D_800F4DB8 + 0x38); i < 4; i++, p += 0x188) {
        if (*p & 1) func_800501BC(i);
    }
    func_80050EC4();
    func_80051378();
    func_80051554();
    func_8004F418();
    func_800518E0();
}
