#include "types.h"
extern int func_80084534();extern int func_8008A644();extern int func_8004F82C();extern int func_80055300();extern int func_80055330();extern int func_8005534C();extern int func_8004FED4();extern int D_800F2EC8;
extern struct { char p0[0x4]; s32 x4; char p1[0x4]; s32 xC; s32 x10; char p2[0x44]; s16 x58; char p3[0x61E]; s16 x678; char p4[0x61E]; s16 xC98; char p5[0x61E]; s16 x12B8; } D_800F4DB8;
extern struct { char p0[0x1C]; s32 x1C; char p1[0x28]; s32 x48; s32 x4C; char p2[0x38C]; s32 x3DC; } D_800AC650;
void func_8004F720(void) {
    func_80084534(0);
    func_8008A644(4);
    D_800F4DB8.xC = 1;
    D_800F4DB8.x4 = 0;
    func_8004F82C(D_800F4DB8.x10, 1);
    func_80055300();
    D_800F2EC8 = 1;
    func_80055330(0, 0x90, 0x90, 0x90);
    func_8005534C(D_800AC650.x3DC);
    func_8004FED4();
    D_800F4DB8.x58 = 0x52;
    D_800F4DB8.x678 = 0x52;
    D_800F4DB8.xC98 = 0x4E;
    D_800F4DB8.x12B8 = 0x4E;
    D_800AC650.x4C = -1;
    D_800AC650.x1C = 1;
    D_800AC650.x48 = 0;
}
