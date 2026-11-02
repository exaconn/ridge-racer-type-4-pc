/* RODATA 0x80010938 0x228 */
#include "types.h"

typedef struct {
    s32 prim;
} Scratchpad;

typedef struct {
    s16 x, y, w, h;
} RectS16;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    s16 s4;
    s16 s6;
    s16 s8;
    s16 sA;
    s8 c0C;
    u8 pad0D[7];
    u8 b14;
    u8 b15;
    u8 b16;
    u8 b17;
    RectS16 rect18;
    s16 s20;
    u8 pad22[2];
} UiElem24;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} FontGlyph4;

extern s32 func_8001F5B4();
extern s32 func_8001FC28();
extern s32 func_8001FCC8();
extern s32 func_8001FD20();
extern s32 func_8001FF84();
extern void func_8003EA04();
extern void func_8003F920();
extern s32 func_80090424();
extern s32 func_800904F4();

extern char D_80010924[];
extern char D_80010928[];
extern char D_8001092C[];

extern FontGlyph4 D_8009FA1C[];
extern FontGlyph4 D_8009FB80[];
extern FontGlyph4 D_8009FC0C[];
extern s8 D_8009FC14[][4];
extern s8 D_8009FC7C[][8];
extern FontGlyph4 D_8009FCDC[];
extern s8 D_8009FD00[];

extern s16 D_800ABEC8;
extern s16 D_800AC59C;
extern s32 D_800ACC14;
extern s16 D_800AD522;
extern s16 D_800F3826;
extern u8 D_800F3920[];
extern s32 D_800F3A30;
extern UiElem24 *D_800F4D80;
extern s32 D_80119754;
extern s32 D_80119758;

s32 func_8003EC88(s32 arg0) {
    RectS16 sp28;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 s0;
    s32 s1;
    s32 s2;
    /* ASM_BARRIER: Keep s3 in $s3 while s3_init has single assignment in prologue */
    register s32 s3 asm("s3");
    /* ASM_BARRIER: Use $0 register operand so reload_cse_regs does not replace 0 with $fp */
    register s32 zero asm("$0");
    s32 s3_init;
    s32 s4;
    s32 s5_1;
    s32 s5_2;
    s32 s6;
    s32 s7;
    s32 fp;
    u32 a0_byte;
    s32 a0_arg;
    FontGlyph4 *glyph;

    fp = 0;
    s3_init = ((Scratchpad *)0x1F800000)->prim;
    sp38 = D_800F4D80[arg0].s4;
    s2 = zero;
    sp30 = D_800F4D80[arg0].b14;
    s4 = sp38;
    a0_byte = D_800F4D80[arg0].b0;
    s7 = D_800F4D80[arg0].s6;
    s6 = D_800F4D80[arg0].b15;
    sp34 = D_800F4D80[arg0].b16;

    switch (sp30) {
    case 0:
        D_800F4D80[arg0].b14 = 1;
        D_800F4D80[arg0].rect18.w = 0;
        D_800F4D80[arg0].rect18.x = D_800F4D80[arg0].s4;
        D_800F4D80[arg0].rect18.y = D_800F4D80[arg0].s6;
        D_800F4D80[arg0].rect18.h = D_800F4D80[arg0].sA;
        D_800F4D80[arg0].b0 = 0;
        D_800F4D80[arg0].c0C = 0;
        break;
    case 1:
        D_800F4D80[arg0].rect18.w = (func_80090424((u16)a0_byte << 6) * D_800F4D80[arg0].s8) / 4096;
        if ((u16)a0_byte >= 16) {
            D_800F4D80[arg0].b0 = 0;
            D_800F4D80[arg0].b14 = 2;
        } else {
            D_800F4D80[arg0].b0++;
        }
        break;
    case 2:
        if (D_800F4D80[arg0].b2 == 1) {
            if (D_800F4D80[arg0].c0C & 2) {
                s2 = func_800904F4((D_800F4D80[arg0].b17 << 4) - 0x2AA);
                fp = (u32)s2 >> 31;
                if (s2 >= 0) {
                    s2 = ((s2 >> 6) + 100) << 1;
                } else {
                    s2 = 0;
                }
            }
            if (D_800F4D80[arg0].s20 == 1) {
                fp = 2;
            }
        }
        break;
    case 3:
        D_800F4D80[arg0].rect18.w = (func_800904F4((u16)a0_byte << 6) * D_800F4D80[arg0].s8) / 4096;
        if ((u16)a0_byte >= 16) {
            D_800F4D80[arg0].b0 = 0;
            D_800F4D80[arg0].b14 = 4;
        } else {
            D_800F4D80[arg0].b0++;
        }
        break;
    case 4:
    case 7:
        if (a0_byte >= 2) {
            D_800F4D80[arg0].b0 = 0;
            if (sp30 == 7) {
                D_800F4D80[arg0].b14 = 8;
            } else if (D_800F4D80[arg0].b1 == 0) {
                D_800F4D80[arg0].b14 = 5;
            } else {
                D_800F4D80[arg0].b14 = 6;
            }
        } else {
            D_800F4D80[arg0].b0++;
        }
        break;
    case 5:
    case 6:
        break;
    case 8:
        if (a0_byte >= 16) {
            D_800F4D80[arg0].b0 = 0;
            D_800F4D80[arg0].b14 = 9;
        } else {
            D_800F4D80[arg0].b0++;
        }
        /* fallthrough */
    case 9:
        if (D_800F4D80[arg0].b2 == 1) {
            fp = (D_800F4D80[arg0].c0C != 1) << 1;
        }
        break;
    }

    sp28 = D_800F4D80[arg0].rect18;
    s3 = func_8001FC28(D_800ACC14 + 0x70, s3_init, 0, 0, 0x140, 0x200);
    s4 += 7;

    switch (s6) {
    case 5:
    case 6:
    case 7:
        if (s6 == 5) {
            s0 = D_800F3A30;
        }
        if (s6 == 6) {
            s0 = D_800AC59C - 2;
        }
        fp = 0;
        if (s6 == 7) {
            s0 = D_800ABEC8 + 1;
        }
        s2 = 0;
        /* fallthrough */
    case 0:
        {
            FontGlyph4 *base_0 = D_8009FA1C;
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &base_0[sp34], fp | 0x7FF8, s2);
            if (s6 != 0) {
                for (s1 = 0; s1 < 2; s1++) {
                    s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4 + 0x1E + s1 * 0x16, s7 + 8, &D_8009FC0C[s1], 0x7FF8, 0);
                    if (s1 == 0) {
                        D_8009FD00[0] = s0 / 10;
                    } else {
                        D_8009FD00[0] = (s0 % 10) * 8;
                    }
                    s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4 + 0x26 + s1 * 6, s7 + 8, D_8009FD00, 0x7FF8, 0);
                }
                /* ASM_BARRIER: Keep base_0 (D_8009FA1C) live across case 0 loop so reload rematerializes it into $t0 */
                __asm__("" : : "ri"(base_0));
            }
        }
        break;
    case 1:
    case 4:
    case 8:
    case 9:
        for (s1 = 0; s1 < 4; s1++) {
            s0 = D_8009FC14[sp34][s1];
            if (s0 == -1) {
                break;
            }
            s5_1 = fp | 0x7FF8;
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], s5_1, s2);
            s4 += 2 + D_8009FA1C[s0].b2;
        }
        if (s6 == 4) {
            if (D_800AD522 == 1) {
                s0 = 0xD;
            } else {
                s0 = 0xC;
            }
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
        } else if ((u32)(s6 - 8) < 2U) {
            s3 = func_8001F5B4(D_800ACC14 + 0x70, s3, s4, s7 + 8, (s6 == 8) ? D_80010924 : D_80010928, fp | 0x7FF8, 0, s2);
        }
        break;
    case 2:
        for (s1 = 0; s1 < 7; s1++) {
            s0 = D_8009FC7C[sp34][s1];
            if (s0 == -1) {
                break;
            }
            a0_arg = D_800ACC14 + 0x70;
            glyph = &D_8009FA1C[s0];
            s5_2 = fp | 0x7FF8;
            s3 = func_8001FF84(a0_arg, s3, s4, s7 + 8, glyph, s5_2, 0);
            s4 += 2 + D_8009FA1C[s0].b2;
        }
        s3 = func_8001FD20(D_800ACC14 + 0x70, s3, sp38 + 0xA9, s7 + 6, 1, 0x14, 0xFF, 0xFF, 0xFF);
        s4 = sp38 + 0xB0;
        for (s1 = 0; s1 < 2; s1++) {
            if (s1 == 0) {
                s0 = 0x45;
            } else {
                s0 = D_8009FC7C[sp34][7];
            }
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
            s4 += 2 + D_8009FA1C[s0].b2;
        }
        break;
    case 3:
        for (s1 = 0; s1 < 7; s1++) {
            s0 = D_8009FC7C[sp34][s1];
            if (s0 == -1) {
                break;
            }
            s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
            s4 += 2 + D_8009FA1C[s0].b2;
            if (s1 == 3) {
                s3 = func_8001FD20(D_800ACC14 + 0x70, s3, sp38 + 0x7E, s7 + 6, 1, 0x14, 0xFF, 0xFF, 0xFF);
                s4 = sp38 + 0x86;
            }
        }
        break;
    case 10:
    case 11:
        s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FB80[0], 0x7FF8, 0);
        s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4 + 0x44, s7 + 8, &D_8009FB80[1], 0x7FF8, 0);
        if (s6 == 10) {
            s0 = D_80119754;
        } else {
            s0 = D_80119758;
        }
        for (s1 = 0; s1 < s0; s1++) {
            s3 = func_8001FD20(D_800ACC14 + 0x70, s3, s4 + 0x12 + s1 * 3, s7 + 8, 2, 0x10, 0xFF, 0xFF, 0xFF);
        }
        break;
    case 12:
        if (D_800F3920[D_800F3826 + 0x5C] == 1) {
            s2 = 8;
        } else {
            s2 = 7;
        }
        if (s2 != 0) {
            s1 = 0;
            do {
                s0 = D_8009FC7C[sp34][s1];
                s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FA1C[s0], 0x7FF8, 0);
                s4 += 2 + D_8009FA1C[s0].b2;
                s1++;
            } while (s1 < s2);
        }
        break;
    case 0x7E:
        s3 = func_8001F5B4(D_800ACC14 + 0x70, s3, s4, s7 + 8, D_8001092C, 0x7FF8, 2, 0);
        break;
    case 0x7F:
        s3 = func_8001FF84(D_800ACC14 + 0x70, s3, s4, s7 + 8, &D_8009FCDC[sp34], fp | 0x4820, s2);
        break;
    }

    ((Scratchpad *)0x1F800000)->prim = func_8001FCC8(D_800ACC14 + 0x70, s3, 0x38);
    func_8003F920(sp38, s7, D_800F4D80[arg0].s8, D_800F4D80[arg0].sA, 0x4A, 0x4A, 0x4A);
    ((Scratchpad *)0x1F800000)->prim = func_8001FC28(D_800ACC14 + 0x70, ((Scratchpad *)0x1F800000)->prim, sp28.x, sp28.y, sp28.w, sp28.h);
    sp38 += D_800F4D80[arg0].rect18.w;
    if ((u32)(sp30 - 4) >= 3U) {
        func_8003EA04(arg0, sp38, s7);
    }
    return (u32)~sp30 >> 31;
}
