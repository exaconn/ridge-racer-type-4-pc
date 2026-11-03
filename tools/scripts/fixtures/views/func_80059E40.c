#include "rr/line_emit.h"

extern void func_8004C5A4(s32, s32, const char *, s32, s32);

extern s32 func_8004C73C(s32, s32, char *, s32, s32);

extern s32 func_8004C7E8(s32, s32, char *, s32, s32);

extern void func_8004C928(s32);

extern void func_80059700(s16 *, s32, s32);

extern void func_80059904(s16 *, s32, s32);

extern void func_80059B1C(u8 *, s32, s32, s32);

extern void func_80059C28(s32, u8);

extern void func_80059D34(s32, u8);

extern void func_80096484(char *, const char *, s32);

extern const char D_80011228[];

extern const char D_8001122C[];

extern const char D_80011230[];

extern const char D_80011234[];

extern const char D_8001123C[];

extern char D_800A095C[];

extern s32 D_800A0C9C[];

extern char D_800A1830[];

extern s16 *D_800A18F0[];

extern s16 *D_800A1910[];

extern s16 *D_800A1930[];

extern s16 *D_800A1970[];

extern s16 *D_800A19A4[];

extern u8 D_800A1A60[];

extern u8 D_800A1A68[];

extern u32 D_800A1A70[];

extern u32 D_800A1A88[];

extern s16 D_800F3914;

extern u16 D_800F3A40;

extern s32 D_800F3A60;

extern struct {
    char pad[0x28A4];
    s32 w28a4;
    s32 w28a8;
    s32 w28ac;
} D_800F4DB8;

extern s32 D_8010E228;

extern s32 D_8010E22C;

extern s32 D_8010E230;

extern s32 D_8010E234;

extern s32 D_8010E238;

s32 func_80059E40(s32 arg0) {
    s32 sp18[6];
    char sp30[16];
    s16 sp40[4];
    u8 sp48[8];
    s16 sp50[4];
    u8 sp58[8];
    s32 i;
    s32 s2;
    s32 s4;
    s32 s0_1;
    s32 s0_2;
    s32 s0_3;

    s2 = D_8010E22C;
    s4 = 0;
    if (arg0 < 0) {
        arg0 = 0;
    }
    if (arg0 >= 0x1C3) {
        s4 = ++D_8010E238 >= 0x4C;
        if (arg0 >= 0xA8D) {
            s4 = 1;
        }
        arg0 = 0x1C2;
    }

    if (s2 < 8) {
        for (i = 0; i < 6; i++) {
            if (D_800A1A70[i] < (u32)arg0) {
                sp18[i] = arg0 - D_800A1A70[i];
                if (sp18[i] < 0x19) {
                    sp18[i] = (sp18[i] * 0x80) / 25;
                } else {
                    sp18[i] = 0x80;
                }
            } else {
                sp18[i] = 0;
            }
            if (D_8010E228 >= 0) {
                sp18[i] = (D_8010E228 * 0x80) / 100;
                if (D_8010E228 > 0) {
                    D_8010E228--;
                } else {
                    return 1;
                }
            }
        }

        sp40[0] = 0x48;
        sp40[1] = 0x48;
        sp40[3] = 0x48;
        if (D_8010E228 < 0) {
            sp40[2] = (sp18[0] * 0xB0) / 128 + 0x47;
            sp48[2] = 0x80;
            sp48[1] = 0x80;
            sp48[0] = 0x80;
        } else {
            sp40[2] = 0xF7;
            sp48[0] = sp48[1] = sp48[2] = sp18[0];
        }
        if (sp40[2] > sp40[0]) {
            func_8004D290(sp40, sp48, 1, 0);
            sp40[1] = 0x47;
            sp40[3] = 0x47;
            sp48[0] = sp48[1] = sp48[2] = sp48[0] >> 1;
            func_8004D290(sp40, sp48, 1, 0);
            sp40[1] = 0x49;
            sp40[3] = 0x49;
            func_8004D290(sp40, sp48, 1, 0);
        }

        func_80059700(D_800A18F0[s2], sp18[1], 0);
        func_80059700(D_800A1910[s2], sp18[1], 0);
        func_80059700(D_800A1930[s2], sp18[2], 0);
        func_80059904(D_800A1970[D_8010E230], sp18[2], 0);
        func_80059D34(0, sp18[3]);
        func_8004C5A4(0x9A, 0xA0, D_80011228, sp18[4], 0);
        s0_1 = func_8004C73C(0x60, 0xB0, D_800A1830 + D_800F3914 * 0x18, sp18[4], 0) / 2;
        func_8004C5A4(0x9A - s0_1, 0xB0, D_8001122C, sp18[4], 0);
        func_8004C5A4(0xA2 + s0_1, 0xB0, D_80011230, sp18[4], 0);
        func_8004C928(0);

        if (sp18[5] >= 0x80) {
            if (D_800F3A40 & 0x8000) {
                if (D_8010E234 == 0) {
                    D_800F4DB8.w28a4 = 0;
                    D_8010E234 = 1;
                } else {
                    D_800F4DB8.w28a4--;
                    if (D_800F4DB8.w28a4 < 0) {
                        D_800F4DB8.w28a4 = D_800F4DB8.w28a8 - 1;
                    }
                }
                D_8010E238 = 0;
            }
            if (D_800F3A40 & 0x2000) {
                if (D_8010E234 == 0) {
                    D_800F4DB8.w28a4 = 0;
                    D_8010E234 = 1;
                } else {
                    D_800F4DB8.w28a4++;
                    if (D_800F4DB8.w28a4 >= D_800F4DB8.w28a8) {
                        D_800F4DB8.w28a4 = 0;
                    }
                }
                D_8010E238 = 0;
            }
            if (D_800F3A60 != 0) {
                s4 = 1;
            }
        }

        func_80059B1C(D_800A1A60, 0x3C, 0xC8, sp18[5]);
        func_80059B1C(D_800A1A60 + 4, 0xFC, 0xC8, sp18[5]);
        func_80059B1C(D_800A1A60 + 8, 0x4C, 0xC8, sp18[5]);
        func_80096484(sp30, D_80011234, D_800F4DB8.w28a4);
        func_8004C5A4(0x5A, 0xC8, sp30, sp18[5], 0);
        func_80059B1C(D_800A1A60 + 0xC, 0x6C, 0xC8, sp18[5]);
        if (D_800F4DB8.w28a4 <= 0) {
            func_8004C5A4(0x78, 0xC8, D_8001123C, sp18[5], 0);
        } else {
            func_8004C5A4(0x78, 0xC8, D_800A095C + (D_800A0C9C[D_800F4DB8.w28a4 - 1] << 5), sp18[5], 0);
        }
        func_8004C928(0);
        return s4;
    } else {
        for (i = 0; i < 6; i++) {
            if (D_800A1A88[i] < (u32)arg0) {
                sp18[i] = arg0 - D_800A1A88[i];
                if (i != 0) {
                    if (sp18[i] < 0x19) {
                        sp18[i] = (sp18[i] * 0x80) / 25;
                    } else {
                        sp18[i] = 0x80;
                    }
                } else {
                    if (sp18[0] < 0xC) {
                        sp18[0] = (sp18[0] * 0x80) / 12;
                    } else {
                        sp18[0] = 0x80;
                    }
                }
            } else {
                sp18[i] = 0;
            }
            if (D_8010E228 >= 0) {
                sp18[i] = (D_8010E228 * 0x80) / 100;
                if (D_8010E228 > 0) {
                    D_8010E228--;
                } else {
                    return 1;
                }
            }
        }

        s2 -= 8;
        func_80059700(D_800A19A4[s2], sp18[1], 0);

        sp50[0] = 0x48;
        sp50[1] = 0x38;
        sp50[3] = 0x38;
        if (D_8010E228 < 0) {
            sp50[2] = (sp18[0] * 0xB0) / 128 + 0x47;
            sp58[2] = 0x80;
            sp58[1] = 0x80;
            sp58[0] = 0x80;
        } else {
            sp50[2] = 0xF7;
            sp58[0] = sp58[1] = sp58[2] = sp18[0];
        }
        if (sp50[2] > sp50[0]) {
            func_8004D290(sp50, sp58, 1, 0);
            sp50[1] = 0x37;
            sp50[3] = 0x37;
            sp58[0] = sp58[1] = sp58[2] = sp58[0] >> 1;
            func_8004D290(sp50, sp58, 1, 0);
            sp50[1] = 0x39;
            sp50[3] = 0x39;
            func_8004D290(sp50, sp58, 1, 0);
        }

        func_8004C5A4(0x9A, 0x48, D_80011228, sp18[2], 0);
        s0_2 = func_8004C73C(0x60, 0x58, D_800A1830 + D_800F3914 * 0x18, sp18[2], 0) / 2;
        func_8004C5A4(0x9A - s0_2, 0x58, D_8001122C, sp18[2], 0);
        func_8004C5A4(0xA2 + s0_2, 0x58, D_80011230, sp18[2], 0);
        func_8004C928(0);

        if (s2 < 3) {
            func_80059C28(1, sp18[3]);
        } else {
            func_80059C28(s2 - 1, sp18[3]);
        }

        if (D_8010E234 >= 0) {
            if (sp18[5] >= 0x80) {
                if (D_800F3A40 & 0x8000) {
                    D_800F4DB8.w28a4--;
                    if (D_800F4DB8.w28a4 < 0) {
                        D_800F4DB8.w28a4 = D_800F4DB8.w28a8 - 1;
                    }
                    D_8010E238 = 0;
                }
                if (D_800F3A40 & 0x2000) {
                    D_800F4DB8.w28a4++;
                    if (D_800F4DB8.w28a4 >= D_800F4DB8.w28a8) {
                        D_800F4DB8.w28a4 = 0;
                    }
                    D_8010E238 = 0;
                }
                if (D_800F3A60 != 0) {
                    s4 = 1;
                }
            }
            D_800F4DB8.w28ac = D_800F4DB8.w28a4;
            func_80059B1C(D_800A1A60, 0x3C, 0xC8, sp18[5]);
            func_80059B1C(D_800A1A60 + 4, 0xFC, 0xC8, sp18[5]);
            func_80059B1C(D_800A1A60 + 8, 0x4C, 0xC8, sp18[5]);
            func_80096484(sp30, D_80011234, D_800F4DB8.w28a4);
            func_8004C5A4(0x5A, 0xC8, sp30, sp18[5], 0);
            func_80059B1C(D_800A1A60 + 0xC, 0x6C, 0xC8, sp18[5]);
            if (D_800F4DB8.w28a4 > 0) {
                func_8004C5A4(0x78, 0xC8, D_800A095C + (D_800A0C9C[D_800F4DB8.w28a4 - 1] << 5), sp18[5], 0);
            } else {
                func_8004C5A4(0x78, 0xC8, D_8001123C, sp18[5], 0);
            }
        } else {
            s0_3 = 0x88 - func_8004C7E8(0x14, 0xC8, D_800A095C + (D_800A0C9C[D_800F4DB8.w28a4 - 1] << 5), sp18[5], 0) / 2;
            func_80059B1C(D_800A1A68, s0_3, 0xC8, sp18[5]);
            func_80096484(sp30, D_80011234, D_800F4DB8.w28a4);
            func_8004C5A4(s0_3 + 0xE, 0xC8, sp30, sp18[5], 0);
            func_80059B1C(D_800A1A68 + 4, s0_3 + 0x20, 0xC8, sp18[5]);
            if (sp18[5] >= 0x80 && D_800F3A60 != 0) {
                s4 = 1;
            }
        }
    }

    func_8004C928(0);
    return s4;
}
