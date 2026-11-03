#include "types.h"
#include "m2c_macros.h"
#include "game/gfx/fadesel.h"

extern struct {
    char pad0[0x3E4]; int f3E4; char pad1[0x3F4 - 0x3E8]; int f3F4; int f3F8;
    char pad2[0x404 - 0x3FC]; int f404; char pad3[0x41C - 0x408]; int f41C;
    char pad4[0x434 - 0x420]; int f434; int f438; int f43C; int f440;
} D_800AC650;
extern s32 D_800ACA78;

void func_8005C988(a0)
int a0;
{
    D_800AC650.f404 = 0;
    D_800AC650.f3F8 = 0;
    D_800AC650.f434 = 1;
    D_800AC650.f43C = 0;
    D_800AC650.f440 = 0;
    switch (a0) {
    case 0:
        D_800AC650.f438 = 0xB;
        break;
    case 1:
        D_800AC650.f438 = 8;
        break;
    case 2:
        D_800AC650.f438 = 0xC;
        break;
    }
    D_800AC650.f3F4 = 1;
    D_800AC650.f3E4 = 0xBB8;
    D_800AC650.f41C = 2;
}

void func_8005CA04(void) {
    D_800ACA78 = 1;
}
