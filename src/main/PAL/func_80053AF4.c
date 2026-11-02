#include "types.h"
extern int func_80053A9C();extern int func_80083C74();extern void func_8007F910(int);extern int func_80096434();
extern struct { char p0[0x2864]; s32 x2864; s32 x2868; s32 x286C; char p1[0x18]; u16 x2888; s16 x288A; s16 x288C; s16 x288E; s16 x2890; } D_800F4DB8;
void func_80053AF4(int a0, int a1) {
    if ((s16)a1 >= 2) {
        u16 v = D_800F4DB8.x2888;
        D_800F4DB8.x288A = a1;
        D_800F4DB8.x288C = 0;
        D_800F4DB8.x288E = a0;
        D_800F4DB8.x2890 = v;
    } else {
        int s = (s16)a0;
        func_80053A9C(s);
        D_800F4DB8.x288A = 0;
        D_800F4DB8.x288C = 0;
        D_800F4DB8.x288E = 0;
        D_800F4DB8.x2890 = 0;
        if (s == 0) {
            if (D_800F4DB8.x2864 >= 0) {
                func_80083C74((s16)D_800F4DB8.x2864);
                func_8007F910((s16)D_800F4DB8.x2864);
                func_80096434(&D_800F4DB8.x286C, 0x1C);
            }
            D_800F4DB8.x2864 = -1;
            D_800F4DB8.x2868 = -1;
        }
    }
}
