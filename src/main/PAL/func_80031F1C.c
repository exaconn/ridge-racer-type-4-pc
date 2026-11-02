#include "types.h"
#include "m2c_macros.h"

extern int func_8002A604();
extern int func_8002AF8C();
extern int func_8003D228();
extern s8 D_800ACC68;
extern s16 D_800F3A26;
extern void *D_800F3AFC;
extern s16 D_800F4C6A;
extern s16 D_800F4D3C;
extern s16 D_800F4D8A;

void func_80031F1C(void) {
    func_8002A604();
    D_800ACC68 = 2;
    D_800F4D3C = 0;
    func_8003D228(1, 0x80, 0x58, 0xA8, 0x80, 0, 0x701);
    M2C_FIELD(D_800F3AFC, s16 *, 0x1E) = 1;
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    M2C_FIELD(D_800F3AFC, s16 *, 2) = 1;
    func_8002AF8C(0, 1, 1);
    D_800F4C6A = 1;
    D_800F3A26 = 0xE;
    D_800F4D8A = 0;
}
