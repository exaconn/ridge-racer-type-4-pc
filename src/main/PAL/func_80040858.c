/* RODATA 0x80010ea0 0x2c */
/* CC1_FLAGS: -fno-strength-reduce */
#include "rr/gpu.h"

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

extern RrSpritePacket *func_8001FCC8();
extern void func_80040CB4(s32);
extern s32 func_80090424();
extern s32 func_800904F4();
extern void func_80095A44();
extern void func_80095B94();

extern char *D_800ACC14;
extern s32 D_800F4D30;
extern RrRow60 *D_800FFC14;

void func_80040858(s32 a0) {
    s32 x[4];
    s32 y[4];
    s32 u[4];
    s32 v[4];
    s32 s6;
    s32 mode;
    s32 s2;
    s32 s1;
    RrSpritePacket *s3;
    RrSpritePacket *s0;

    if (a0 < D_800F4D30) {
        mode = D_800FFC14[a0].unk2;
        s6 = D_800FFC14[a0].unk0;
        x[0] = D_800FFC14[a0].unk38;
        y[0] = D_800FFC14[a0].unk3A;
        switch (mode) {
        case -1:
            break;
        case 0:
            D_800FFC14[a0].unk0 = 0;
            D_800FFC14[a0].unk2 = 1;
            break;
        case 1:
        case 8:
            u[0] = u[2] = 0xF4;
            u[1] = u[3] = 0xFF;
            v[0] = v[1] = 0x68;
            v[2] = v[3] = 0x7F;
            y[2] = y[0] + 0x18;
            if (mode == 1) {
                s2 = func_80090424((s6 << 10) / 30);
            } else {
                s2 = func_800904F4((s6 << 10) / 30);
            }
            s2 /= 32;
            s3 = *(RrSpritePacket **)0x1F800000;
            s0 = s3;
            for (s1 = 0; s1 < 0x16; s0++) {
                func_80095B94(s3);
                s3++;
                s0->x = x[0];
                s0->y = y[0];
                s0->w = 0xC;
                s0->h = 0x18;
                s0->u = 0xF4;
                s0->v = 0x68;
                s0->r = s2;
                s0->g = s2;
                s0->b = s2;
                s0->clut = 0x2678;
                func_80095A44(D_800ACC14 + 0x70, s0);
                s1++;
                x[0] += 0xD;
            }
            *(RrSpritePacket **)0x1F800000 = func_8001FCC8(D_800ACC14 + 0x70, s3, 0x2E);
            if (s6 >= 0x1E) {
                D_800FFC14[a0].unk2++;
                D_800FFC14[a0].unk0 = 0;
            } else {
                D_800FFC14[a0].unk0++;
            }
            break;
        case 2:
            D_800FFC14[a0].unk3 = 0;
            for (s1 = 0; s1 < 0x16; s1++) {
                D_800FFC14[a0].unk4[s1] = 0;
            }
            if ((u16)(D_800FFC14[a0].unk34 - 1) < 0xFF) {
                D_800FFC14[a0].unk2 = 4;
            } else if ((s16)D_800FFC14[a0].unk34 >= 0) {
                D_800FFC14[a0].unk2 = 3;
            } else {
                D_800FFC14[a0].unk2 = -1;
            }
        case 3:
            if (s6 >= 2) {
                if (D_800FFC14[a0].unk3 < 0x16) {
                    D_800FFC14[a0].unk3++;
                    D_800FFC14[a0].unk4[D_800FFC14[a0].unk3] = 0;
                    D_800FFC14[a0].unk0 = 0;
                } else if (s6 >= 0x40) {
                    D_800FFC14[a0].unk0 = 0;
                } else {
                    D_800FFC14[a0].unk0++;
                }
            } else {
                D_800FFC14[a0].unk0++;
            }
            func_80040CB4(a0);
            break;
        case 4:
            D_800FFC14[a0].unk3 = 0x16;
            func_80040CB4(a0);
            break;
        case 6:
            D_800FFC14[a0].unk3 = 1;
            func_80040CB4(a0);
            break;
        case 9:
            break;
        }
    }
}
