/* RODATA 0x80010590 0x20 */
#include "types.h"

typedef struct {
    u8 pad0[0x16];
    s16 unk16;
    u8 pad18[0x4];
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    u8 unk1F;
    s32 unk20;
} Struct800FF68C;

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[0x18];
} Struct800F3AFC;

extern s32 func_8001FCC8(s32, s32, s32);
extern s32 func_8001FD20(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_8002018C(s32, s32, void *, s32, s32, s32);
extern s32 func_80020508(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_8003DC9C(s32, s32, s32, s32, s32, s32, s32);
extern s32 func_8003DCEC(s32);
extern void func_80043608(s32);

extern u8 D_8009F3F8[];
extern u8 D_8009F404[];
extern s32 D_800ACC14;
extern Struct800F3AFC *D_800F3AFC;
extern Struct800FF68C *D_800FF68C;

void func_8003189C(void) {
    s32 var_a1;
    s32 temp_v0;
    s32 temp_s2;

    temp_v0 = D_800FF68C->unk1E;
    temp_s2 = D_800FF68C->unk1F;
    switch (temp_v0) {
    case -2:
        D_800FF68C->unk1F = 0;
        D_800FF68C->unk1E = -4;
        D_800FF68C->unk1D = -1;
        D_800FF68C->unk1C = -1;
        break;
    case -4:
        func_8003DC9C(0, 0x78, 0x60, 0x50, 0xB4, 0, 0);
        D_800FF68C->unk1E = -5;
        break;
    case -5:
        if (func_8003DCEC(0) != 0) {
            D_800FF68C->unk1E = -1;
        }
        break;
    case -3:
        break;
    case -1:
        {
            s32 a1 = *(s32 *)0x1F800000;
            *(s32 *)0x1F800000 = func_8001FCC8(
                D_800ACC14 + 0x74,
                func_80020508(D_800ACC14 + 0x74, a1, 0x78, 0x60, 0x50, 0xB4, (u16)temp_s2, (u16)temp_s2, (u16)temp_s2),
                0x45);
            if ((u16)temp_s2 >= 0x7C) {
                D_800FF68C->unk1E = 2;
                D_800FF68C->unk1F = 0;
                D_800F3AFC[D_800FF68C->unk16].unk2 = 1;
            } else {
                D_800FF68C->unk1F += 8;
            }
            func_8003DCEC(0);
        }
        break;
    case 0:
        var_a1 = *(s32 *)0x1F800000;
        if ((u16)temp_s2 >= 3) {
            var_a1 = func_8002018C(D_800ACC14 + 0x70, var_a1, D_8009F3F8, 1, 0xFF, 2);
        }
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x74, var_a1, 0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
        if ((u16)temp_s2 >= 0x15) {
            D_800FF68C->unk1E = 1;
            D_800FF68C->unk1F = 0;
        } else {
            D_800FF68C->unk1F += 1;
        }
        if (temp_s2 == 1) {
            u8 temp_a0 = D_800FF68C->unk1C;
            if ((u32)(temp_a0 - 1) < 5U) {
                func_80043608(D_800FF68C->unk20 + (((s8)temp_a0 - 1) << 14));
            }
        }
        break;
    case 1:
        {
            s32 two = 2;
            *(s32 *)0x1F800000 = func_8001FD20(
                D_800ACC14 + 0x7C,
                func_8002018C(
                    D_800ACC14 + 0x70,
                    func_8002018C(D_800ACC14 + 0x70, *(s32 *)0x1F800000, D_8009F404, 1, temp_s2, 1),
                    D_8009F3F8, 1, (0x80 - temp_s2) * 2, two),
                0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
            if ((u32)temp_s2 >= 0x80U) {
                D_800FF68C->unk1F = 0;
                D_800FF68C->unk1E = two;
            } else {
                D_800FF68C->unk1F += 8;
            }
        }
        break;
    case 2:
    default:
        var_a1 = *(s32 *)0x1F800000;
        if (D_800FF68C->unk1C >= 0) {
            var_a1 = func_8002018C(D_800ACC14 + 0x70, var_a1, D_8009F404, 0, 0x80, 1);
        }
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x74, var_a1, 0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
        break;
    }
}
