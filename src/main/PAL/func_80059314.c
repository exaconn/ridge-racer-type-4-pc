#include "types.h"

/* RODATA 0x80011200 0x1c */

extern void func_8004F9C8(void);
extern void func_80059284(s16 *, void *);

extern u8 D_800A1090[];
extern u8 D_800A1158[];
extern s16 D_800A1818[];
extern s16 D_800A1824[];
extern s16 *D_800A18F0[];
extern s16 *D_800A1910[];
extern s16 *D_800A1930[];
extern s16 *D_800A1950[];
extern s16 *D_800A1970[];
extern s16 *D_800A1984[];
extern s16 *D_800A19A4[];
extern s16 *D_800A19C0[];
extern s32 D_800A19E0[];
extern s32 D_800A1A00[];
extern s32 D_800A1A20[];
extern s32 D_800A1A40[];

extern s16 D_800AD520;
extern s16 D_800F3914;
extern s32 D_800F3970;
extern s16 D_800F4D44;

extern struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} D_800F4D98[];

extern struct {
    char pad[0x28A4];
    s32 w28a4;
    s32 w28a8;
    s32 w28ac;
} D_800F4DB8;

extern s32 D_800F765C;
extern s32 D_800F7660;
extern s32 D_8010E220;
extern s32 D_8010E228;
extern s32 D_8010E22C;
extern s32 D_8010E230;
extern s32 D_8010E234;
extern s32 D_8010E238;

void func_80059314(s32 arg0, s32 arg1) {
    s32 i;

    D_8010E22C = arg0;
    D_8010E230 = arg1;
    D_8010E228 = -1;
    D_8010E234 = 0;
    D_8010E238 = 0;
    func_8004F9C8();

    if (D_800F4D44 != 1) {
        D_800F765C = 0;
    } else {
        switch (D_800AD520) {
        case 0:
            D_800F765C = D_800A1A40[D_800F3914] + 1;
            break;
        case 1:
            D_800F765C = D_800A1A00[D_800F3914] + 1;
            break;
        case 2:
            D_800F765C = D_800A19E0[D_800F3914] + 1;
            break;
        case 3:
            D_800F765C = D_800A1A20[D_800F3914] + 1;
            break;
        }
    }

    switch (D_800F4D44) {
    case 0:
    case 2:
    case 3:
    case 6:
        D_800F4DB8.w28a4 = D_800F4DB8.w28ac;
        for (i = 0; i < 2; i++) {
            if (D_800F4D98[i].unk1 == 0 && D_800F4D98[i].unk0 == 2 && D_800F4D98[i].unk2 == 0x27) {
                D_800F4DB8.w28a4 = 0xF;
            }
        }
    case 1:
    case 5:
        if (D_800F3970 != 0 && D_800F4D44 != 1) {
            D_800F7660 = 0x10;
        } else {
            D_800F7660 = 0xF;
        }
        break;
    case 4:
    default:
        D_8010E234 = -1;
        D_800F4DB8.w28a4 = D_800A19E0[D_800F3914] + 1;
        D_800F4DB8.w28a8 = 0;
        break;
    }

    D_8010E220 = D_800F765C + 0xB;

    for (i = 0; i < 8; i++) {
        func_80059284(D_800A18F0[i], D_800A1090);
        func_80059284(D_800A1910[i], D_800A1090);
        func_80059284(D_800A1930[i], D_800A1090);
        func_80059284(D_800A1950[i], D_800A1090);
    }

    for (i = 0; i < 5; i++) {
        func_80059284(D_800A1970[i], D_800A1090);
    }

    for (i = 0; i < 8; i++) {
        func_80059284(D_800A1984[i], D_800A1158);
        func_80059284(D_800A1984[i] + 8, D_800A1158);
        func_80059284(D_800A1984[i] + 14, D_800A1158);
    }

    for (i = 0; i < 7; i++) {
        func_80059284(D_800A19A4[i], D_800A1090);
    }

    func_80059284(D_800A1818, D_800A1158);
    func_80059284(D_800A1824, D_800A1158);

    for (i = 0; i < 8; i++) {
        func_80059284(D_800A19C0[i], D_800A1158);
    }
}
