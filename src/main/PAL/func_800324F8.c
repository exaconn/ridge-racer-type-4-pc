#include "types.h"
#include "m2c_macros.h"

extern int func_8002A344();
extern int func_8003D228();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern s16 D_800AC64A;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern void *D_800F3AFC;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern s16 D_800FFC00;
extern s16 D_800FFC18;

void func_800324F8(void) {
    D_800F2DE4 = 0;
    D_800FFC00 = 0;
    D_800FFC18 = 3;
    D_800AC64A = 0;
    func_8003E928(0, 0x8C, 0x88, 0x1C, 0x20, 0, 0, 1, 0, 0x68);
    func_8003E928(1, 0x8C, 0xB0, 0x1C, 0x20, 0, 0, 1, 0, 0x69);
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    D_800F4C6A = 1;
    D_800F3A26 = 0x2B;
    D_800F4D8A = 0;
    M2C_FIELD(D_800F3AFC, s16 *, 2) = 1;
    func_8002A344(0x10086);
}
