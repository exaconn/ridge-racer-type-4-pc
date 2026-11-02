/* RODATA 0x80010e20 0x7c */
#include "types.h"
#include "rr/time_record_types.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    s8 unk3;
    u8 unk4[24];
    u8 unk1C[24];
    u16 unk34;
    u16 unk36;
    s16 unk38;
    s16 unk3A;
} RrRow60;

extern void func_8002B75C(u8 *, char *);
extern void func_8002B814(u8 *, s32, s32);
extern void func_8002B934(u8 *, s32);

extern char D_80010BF0[];
extern char D_80010E0C[];
extern u8 *D_8009FD04[];
extern char *D_8009FD8C[];
extern char *D_8009FD9C;
extern s8 D_800AD4D8[];
extern s8 D_800AD4F1;
extern s16 D_800AD522;
extern RrTimeRecord16 D_800AD5C8[2][8][5];
extern s16 D_800F3914;
extern u8 *D_800F39C8[];
extern s32 D_800F4C64;
extern s32 D_800F4D30;
extern RrTimeRecord16 D_800FF700[2][8][5];
extern RrRow60 *D_800FFC14;

void func_80040338(s32 a0, s16 a1, s32 a2, s32 a3, s32 arg4) {
    s32 t3;
    s32 s2;
    s32 i;
    s32 s0;
    s32 v1;

    if (a0 < D_800F4D30) {
        t3 = arg4 & 0xFF;
        s2 = arg4 >> 8;
        D_800FFC14[a0].unk2 = a2;
        i = 0;
        D_800FFC14[a0].unk1 = a3;
        D_800FFC14[a0].unk38 = 0x11;
        D_800FFC14[a0].unk3A = a1 * 28 + 0x60;
        D_800FFC14[a0].unk36 = 0x2638;
        if (arg4 >= 0) {
            while (D_8009FD04[t3][i] != 0) {
                D_800FFC14[a0].unk1C[i] = D_8009FD04[t3][i];
                i++;
            }
        } else {
            for (; i < 0x16; i++) {
                D_800FFC14[a0].unk1C[i] = 0;
            }
            return;
        }
        switch (s2) {
        case 1:
        case 2:
        case 3:
            func_8002B814(&D_800FFC14[a0].unk1C[4], s2, 0);
        case 4:
            func_8002B934(&D_800FFC14[a0].unk1C[7], D_800F4C64);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            s0 = (s2 - 5) % 5;
            func_8002B814(&D_800FFC14[a0].unk1C[0], s0 + 1, 0);
            v1 = s0;
            if (s0 >= 3) {
                v1 = 3;
            }
            func_8002B75C(&D_800FFC14[a0].unk1C[1], D_8009FD8C[v1]);
            func_8002B75C(&D_800FFC14[a0].unk1C[4], D_80010E0C);
            if (s2 < 10) {
                func_8002B75C(&D_800FFC14[a0].unk1C[14], D_800FF700[D_800AD522][D_800F3914][s0].name);
                func_8002B934(&D_800FFC14[a0].unk1C[4], D_800FF700[D_800AD522][D_800F3914][s0].time);
            } else {
                func_8002B75C(&D_800FFC14[a0].unk1C[14], D_800AD5C8[D_800AD522][D_800F3914][s0].name);
                func_8002B934(&D_800FFC14[a0].unk1C[4], D_800AD5C8[D_800AD522][D_800F3914][s0].time);
            }
            break;
        case 15:
            s0 = D_800F39C8[0][0x1F3];
            func_8002B814(&D_800FFC14[a0].unk1C[12], s0 + 1, 0);
            break;
        case 16:
            func_8002B814(&D_800FFC14[a0].unk1C[8], D_800AD4D8[D_800AD4D8[0x17] + 8], 0);
            break;
        case 17:
            s0 = D_800AD4F1;
            func_8002B75C(&D_800FFC14[a0].unk1C[0], D_80010BF0);
            if (s0 == 0) {
                s0 = 1;
            }
            if (s0 < 9) {
                func_8002B814(&D_800FFC14[a0].unk1C[1], s0, 0);
                s0--;
                if (s0 >= 3) {
                    s0 = 3;
                }
                func_8002B75C(&D_800FFC14[a0].unk1C[2], D_8009FD8C[s0]);
            } else {
                func_8002B75C(&D_800FFC14[a0].unk1C[1], D_8009FD9C);
            }
            break;
        case 18:
        case 19:
        case 20:
        case 21:
            s0 = s2 - 18;
            func_8002B814(&D_800FFC14[a0].unk1C[1], s0 + 1, 0);
            if (s0 >= 3) {
                s0 = 3;
            }
            func_8002B75C(&D_800FFC14[a0].unk1C[2], D_8009FD8C[s0]);
            break;
        case 30:
            D_800FFC14[a0].unk36 = 0x2639;
            D_800FFC14[a0].unk34 = 3;
            break;
        case 31:
            D_800FFC14[a0].unk36 = 0x2639;
            D_800FFC14[a0].unk34 = 2;
            D_800FFC14[a0].unk1C[0] = D_8009FD04[t3][0];
            D_800FFC14[a0].unk1C[1] = 0;
            break;
        }
    }
}
