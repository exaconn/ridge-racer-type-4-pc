#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} Glyph4;

extern s32 func_8001FC28();
extern s32 func_8001FCC8();
extern s32 func_8001FF84();

extern Glyph4 D_8009F6AC[];
extern u16 D_8009F6CC[];
extern u16 D_8009F6D0[];
extern u8 D_8009F6D8[];
extern u8 D_8009F6DA;
extern u8 D_8009F6DB;
extern s32 D_800ACC14;
extern s16 D_800F4D44;
extern s32 D_800FF6B0;

void func_80033F3C(s32 arg0, s32 arg1) {
    Glyph4 sp20;
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 s7;
    s32 fp;
    s32 a1;

    if (D_800FF6B0 != 0 && (arg1 - 0x4B) < 0xAF) {
        s5 = arg1 % 25;
        s3 = (arg1 - 0x4B) / 25;
        if ((D_800ACC14 + 0x70) == arg0) {
            s4 = 0;
            if (s3 == 6) {
                s4 = -(s5 << 7) / 25;
            }
        } else {
            *(s32 *)0x1F800000 = func_8001FC28(arg0, *(s32 *)0x1F800000, 0, 0, 0x140, 0x100);
            s4 = 0x40;
        }
        a1 = func_8001FCC8(arg0, *(s32 *)0x1F800000, 5);
        if (s3 == 1) {
            fp = ((s5 << 6) / 25) + 0x40;
        } else if (s3 >= 4) {
            fp = 0xA4;
            if (s3 >= 5) {
                fp = 0x80;
            }
        } else {
            fp = 0x80;
            if (s3 <= 0) {
                fp = 1;
            }
        }
        if (D_800F4D44 == 1) {
            if (s3 >= 4) {
                s7 = ((arg1 >> 2) & 1) + 2;
                sp20 = D_8009F6AC[7];
                a1 = func_8001FF84(arg0, a1, D_8009F6DB + 0x60, s4 + 0x39, &sp20, D_8009F6D0[s7], 0);
            } else if (s3 > 0) {
                s2 = 0;
                for (s1 = 0; s1 < 0x10;) {
                    s32 b, c;

                    if (s1 > s5 - 7) {
                        break;
                    }
                    b = s1 & 1;
                    c = b << 1;
                    a1 = func_8001FF84(arg0, a1, s2 + 0x78, s4 + 0x39, &D_8009F6AC[2 + b], D_8009F6D0[b * 2], 0);
                    s1++;
                    s2 += 1 << (c + 1);
                }
                s7 = 0;
                sp20 = D_8009F6AC[5];
                sp20 = D_8009F6AC[6];
                sp20.b0 += (s3 - 1) * 12;
                a1 = func_8001FF84(arg0, a1, D_8009F6DA + 0x60, s4 + 0x39, &sp20, D_8009F6D0[0], 0);
            }
        } else {
            if (s3 == 0) {
                s2 = 0;
                for (s1 = 0; s1 < 0x10;) {
                    s32 b, c;

                    if (s1 > s5 - 6) {
                        break;
                    }
                    b = s1 & 1;
                    c = b << 1;
                    a1 = func_8001FF84(arg0, a1, s2 + 0x78, s4 + 0x39, &D_8009F6AC[2 + b], D_8009F6D0[b * 2], 0);
                    s1++;
                    s2 += 1 << (c + 1);
                }
            } else if (s3 >= 4) {
                s7 = ((arg1 >> 2) & 1) + 2;
                sp20 = D_8009F6AC[5];
                sp20.b0 += 0x60;
                for (s1 = 0; s1 < 3; s1++) {
                    if (s1 == 2) {
                        sp20 = D_8009F6AC[7];
                        s1 = 3;
                    }
                    a1 = func_8001FF84(arg0, a1, D_8009F6D8[s1] + 0x60, s4 + 0x39, &sp20, D_8009F6D0[s7], 0);
                }
            } else {
                s7 = 0;
                sp20 = D_8009F6AC[5];
                for (s1 = 0; s1 < 3; s1++) {
                    if (s1 == 0) {
                        sp20.b0 += ((s5 * 9) / 25) * 12;
                    } else if (s1 == 1) {
                        sp20.b0 = D_8009F6AC[5].b0 + s3 * 0x18;
                    } else {
                        sp20 = D_8009F6AC[6];
                        sp20.b0 += (s3 - 1) * 12;
                    }
                    a1 = func_8001FF84(arg0, a1, D_8009F6D8[s1] + 0x60, s4 + 0x39, &sp20, D_8009F6D0[s7], 0);
                }
            }
        }
        if (s3 > 0) {
            s0 = s4 + 0x39;
            s2 = 0;
            for (s1 = 0; s1 < 10; s1++) {
                a1 = func_8001FF84(arg0, a1, s2 + 0x78, s0, &D_8009F6AC[4], D_8009F6D0[(u32)(s1 - 3) < 4 ? 3 : s7], 0);
                s2 += 8;
            }
        }
        a1 = func_8001FF84(arg0, a1, 0x68, s4 + 0x1A, &D_8009F6AC[1], D_8009F6CC[s3 >= 4], (fp << 1) | 1);
        a1 = func_8001FF84(arg0, a1, 0x60, s4, &D_8009F6AC[0], 0x797C, 0);
        *(s32 *)0x1F800000 = func_8001FCC8(arg0, a1, 0x3F);
        if (arg0 == D_800ACC14 + 0xB70) {
            s32 y;
            if (s3 == 6) {
                fp = ((0x19 - s5) * 0x18) / 25;
            } else {
                fp = 0x18;
            }
            y = (0x18 - fp) / 2 + 0x34;
            *(s32 *)0x1F800000 = func_8001FC28(arg0, *(s32 *)0x1F800000, 0x70, s4 + y, 0x60, fp);
        }
    }
}
