/* RODATA 0x80011108 0x10 */
#include "types.h"
extern int func_80084534();extern int func_8008A674();extern int func_8008A644();extern int func_8004FBF0();extern int func_80050E58();extern int func_80051248();extern int func_800514FC();extern int func_8004F2A0();extern int func_800517D8();extern int func_8004EE08();extern int func_8004FB00();
extern struct { char pad[4]; s32 x4; char p2[4]; s32 xC; char p3[0x24-0x10]; s16 v[1]; char p4[0x2868-0x26]; s32 x2868; } D_800F4DB8;
void func_8004F600(void) {
    int idx[4] = {1, 4, 5, 6};
    int i;
    func_80084534(0);
    for (i = 0; i < 4; i++) {
        if (D_800F4DB8.v[idx[i]] >= 0) {
            func_8008A674(D_800F4DB8.v[idx[i]]);
            D_800F4DB8.v[idx[i]] = -1;
        }
    }
    func_8008A644(0x18);
    D_800F4DB8.xC = 0;
    func_8004FBF0();
    func_80050E58();
    func_80051248(0);
    func_800514FC();
    func_8004F2A0();
    func_800517D8();
    func_8004EE08();
    func_8004FB00();
    func_80084534(0);
    D_800F4DB8.x2868 = -1;
    D_800F4DB8.x4 = 0;
}
