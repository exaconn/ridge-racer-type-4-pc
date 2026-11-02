#include "types.h"

/* Versus (serial link) race frame: drives the shared race state machine,
 * exchanges pause/menu input through the serial packet bytes and renders
 * one or two viewports. */

typedef struct {
    u8 pad000[0x1D8];
    s16 speed1D8;
    u8 pad1DA[0x1E8 - 0x1DA];
    s16 active1E8;
    s16 flag1EA;
    u8 pad1EC[0x1EE - 0x1EC];
    u16 rank1EE;
    u8 pad1F0[0x27A - 0x1F0];
    s16 half27A;
    u8 pad27C[0x2AA - 0x27C];
    s16 lap2AA;
    u8 pad2AC[0x320 - 0x2AC];
} Ovl667Vehicle;

typedef struct {
    s32 words[4];
} Ovl667Vector16;

typedef struct {
    u32 pad00[6];
    u8 enable18;
    u8 pad19[0x22778 - 0x19];
} Ovl667FrameStorage;

extern s16 D_80107178;
extern s32 D_80107190;
extern char *D_800ACC14;
extern u32 D_800F2DE4;
extern s32 D_800FF6B0;
extern s32 D_800F4C64;
extern s16 D_800AC5D0;
extern s16 D_800AC59C;
extern s32 D_800AC5A4;
extern s32 D_800ABEB0;
extern s16 D_800AD508;
extern u16 D_800F3A40;
extern s8 D_800F4C68;
extern s8 D_800F2FDC;
extern s16 D_800F4D44;
extern s16 D_800F4C70[];
extern s16 D_800F2AB4[];
extern u16 D_8010C0C8[];
extern Ovl667Vehicle D_800ABEF8[];
extern Ovl667Vehicle *D_800FFC20[];
extern Ovl667FrameStorage D_800ADAF0[2];

extern s32 ovl667_D_80118ED4;
extern s32 ovl667_D_80118ECC;
extern u8 ovl667_D_80118F8D, ovl667_D_80118F8C;
extern u8 ovl667_D_80118EEC, ovl667_D_80118EED, ovl667_D_80118EEE;
extern Ovl667Vector16 ovl667_D_80118E88[];
extern Ovl667Vector16 ovl667_D_80118EA8[];

extern void func_80014A8C();
extern char *func_8001FC28();
extern char *func_8001FD20();
extern void func_800207D4();
extern void func_80020B10();
extern void func_800211A4();
extern void func_80021350();
extern void func_80021684();
extern void func_800217D0();
extern void func_800219BC();
extern void func_80021A7C();
extern void func_80021C00();
extern void func_80021C94();
extern void func_80021E2C();
extern void func_80022098();
extern void func_80022390();
extern void func_800225DC();
extern void func_80022CD4();
extern void func_8002BD18();
extern void func_8002E0D8();
extern void func_80033474();
extern void func_80033BE4();
extern void func_80033F3C();
extern void func_80034630();
extern void func_800347BC();
extern void func_800348E8();
extern void func_800349F4();
extern void func_80036030();
extern void func_80036068();
extern void func_8003660C();
extern void func_80036FD4();
extern void func_80037D88();
extern void func_8003C96C();
extern void func_8004F24C();
extern void func_8004FA04();
extern void func_8004FA50();
extern void func_8004FC18();
extern void func_8004FED4();
extern void func_8005007C();
extern void func_80052A30();
extern void func_8006E1AC();
extern void func_8006ECE8();
extern void func_8006EF8C();
extern void func_80070734();
extern void func_800735D8();
extern void func_800747EC();
extern void func_80092CB8();
extern void func_8009AEAC();
extern void ovl667_func_80114A40(void);
extern void ovl667_func_80115068(void);
extern void ovl667_func_801151E0(void);
extern void ovl667_func_801153F0(void);
extern void ovl667_func_80115508();

#define SCRATCH_CURSOR (*(char **)0x1F800000)

void ovl667_func_80115640(void) {
    s32 split;
    char *frame;
    s32 fade;
    s32 i;
    s32 n;
    s32 state;
    u16 buttons;

    split = (D_80107178 != 2) << 1;
    if (split == 0) {
        frame = D_800ACC14 + 0x70;
    } else {
        *(s16 *)0x1F80005E = 1;
        frame = D_800ACC14 + 0xB70;
    }
    if (D_800F2DE4 < 2) {
        func_80092CB8(0);
    } else {
        func_80092CB8(1);
    }
    func_80014A8C(-1);
    if (ovl667_D_80118ED4 == 2) {
        ovl667_func_80114A40();
        ovl667_D_80118ED4 = 3;
    } else if (ovl667_D_80118ED4 == 3) {
        func_8009AEAC(1, 1, 3);
    }
    ovl667_func_80115068();

    if (D_800F2DE4 >= 75 && D_800FF6B0 == 0) {
        D_800FF6B0 = 1;
        func_8004FED4();
    } else if (D_800FF6B0 == 1) {
        if (D_800F2DE4 >= 176) {
            for (i = 0; i < 4; i++) {
                func_80022CD4(D_800FFC20[i]);
            }
            D_800FF6B0 = 2;
            func_80052A30(1);
            D_800AC5D0 = 30;
        }
    } else if (D_800FF6B0 >= 4) {
        fade = 37;
        if (D_800F4C64 == -1) {
            fade = 69;
        }
        if (D_800AC5D0 < 25) {
            func_800207D4(frame, (25 - D_800AC5D0) * 10, fade);
            SCRATCH_CURSOR = func_8001FC28(frame, SCRATCH_CURSOR, 0, 0, 320, 256);
        }
        if (D_800AC5D0 <= 0) {
            if (ovl667_D_80118ED4 != 3) {
                ovl667_func_80114A40();
            }
            for (i = 0; i < D_800AC59C; i++) {
                D_8010C0C8[i] = D_800FFC20[i]->rank1EE;
            }
            func_8003660C();
            func_8009AEAC(1, 1, 3);
        } else {
            D_800AC5D0--;
        }
    } else if (D_800F2DE4 >= 51 && D_800FF6B0 == 0) {
        func_800207D4(frame, (D_800F2DE4 - 50) * 10, 37);
        SCRATCH_CURSOR = func_8001FC28(frame, SCRATCH_CURSOR, 0, 0, 320, 256);
    }

    for (i = 0; i < D_800AC59C; i++) {
        ovl667_func_80115508(D_800FFC20[i]);
    }
    func_8003C96C();

    if (ovl667_D_80118F8D != ovl667_D_80118F8C) {
        buttons = D_800F3A40;
        ovl667_D_80118EEC = (buttons & 0x800) && D_800AC5D0 == 0;
        ovl667_D_80118EEC |= (buttons & 0x1000) ? 2 : 0;
        ovl667_D_80118EEC |= (buttons & 0x4000) ? 4 : 0;
        state = D_800FF6B0;
        if ((u32)(state - 1) < 3) {
            if ((ovl667_D_80118EEE | ovl667_D_80118EED) & 1) {
                D_800F4C68 ^= 1;
                ovl667_D_80118ECC = ovl667_D_80118F8D;
                if (D_800F4C68 == 0) {
                    D_800AC5D0 = 10;
                    func_8004FA50();
                    func_800348E8(D_800F4D44);
                    if (D_800F2FDC != 0) {
                        ovl667_func_80114A40();
                        func_80036068();
                        return;
                    }
                } else {
                    func_8004FA04();
                }
                D_800F2FDC = 0;
            } else if (D_800AC5D0 > 0 && state < 4) {
                D_800AC5D0--;
            }
        }
        if (D_800F4C68 == 0) {
            D_800F2DE4++;
            D_800ABEB0++;
            if ((u32)(D_800FF6B0 - 2) < 2) {
                if (--D_800AC5A4 <= 0) {
                    D_800FF6B0 = 5;
                    D_800AC5D0 = 75;
                    func_8004FC18(0, 33);
                }
            }
            func_80037D88(4);
        } else {
            if ((ovl667_D_80118EEE | ovl667_D_80118EED) & 2) {
                if (D_800F2FDC > 0) {
                    func_8004F24C(0);
                    D_800F2FDC--;
                }
            }
            if ((ovl667_D_80118EEE | ovl667_D_80118EED) & 4) {
                if (D_800F2FDC < 2) {
                    func_8004F24C(0);
                    D_800F2FDC++;
                }
            }
            func_800349F4(frame, D_800F2FDC);
            func_800347BC();
        }
        ovl667_D_80118F8C = ovl667_D_80118F8D;
    } else {
        ovl667_D_80118EEC = 0;
    }

    n = 1;
    ovl667_func_801151E0();
    if (split != 0) {
        n = 2;
    }
    if (D_800FF6B0 < 4) {
        func_80033F3C(frame, D_800F2DE4);
        if (D_800F4C68 == 0) {
            func_80034630(D_800F2DE4);
        }
    }
    if ((u32)(D_800FF6B0 - 1) < 3) {
        for (i = 0; i < n; i++) {
            if (i == 1 && D_800ABEF8[i].active1E8 == -1) {
                SCRATCH_CURSOR = func_8001FD20(frame, SCRATCH_CURSOR, 0, 128, 320, 128, 8, 8, 8);
            }
            if (n == 1) {
                if (D_800ABEF8[i].active1E8 == -1) {
                    continue;
                }
                func_80021350(D_800F4C70[i], i);
                func_80021E2C(0);
                func_80021C94(0, D_800ABEF8[i].lap2AA);
                func_80022098(frame, 0);
            } else {
                func_800217D0(D_800F4C70[i], i);
                func_80021E2C(i + 1);
                func_80021C94(i + 1, D_800ABEF8[i].lap2AA);
                func_80022098(frame, i + 1);
            }
            func_80021684((D_800ABEF8[i].speed1D8 * 160) / 1168, i);
            func_800219BC(D_800ABEF8[i].half27A, i);
            func_80021A7C((s16)D_800ABEF8[i].rank1EE, i);
            func_8002BD18(D_800AC5A4, i);
            func_80021C00(i + 1);
            if (i == 1) {
                func_80022390(0);
            }
        }
        func_800211A4(split);
        func_80020B10(3);
    }

    for (i = 0; i < n; i++) {
        if (D_800ABEF8[i].active1E8 != -1) {
            *(Ovl667Vector16 *)0x1F800018 = ovl667_D_80118E88[i];
            *(Ovl667Vector16 *)0x1F800008 = ovl667_D_80118EA8[i];
            func_8006EF8C(split + i);
            if (D_800FF6B0 == 0 && D_800F2DE4 < 75) {
                func_80033474(&D_800ABEF8[i], D_800F2DE4);
            } else {
                func_80033BE4(&D_800ABEF8[i]);
            }
            ovl667_D_80118E88[i] = *(Ovl667Vector16 *)0x1F800018;
            ovl667_D_80118EA8[i] = *(Ovl667Vector16 *)0x1F800008;
            func_8006E1AC();
            func_8002E0D8(i + ((s16)(D_800AD508 ^ 1) + 1) * 2);
            if (D_800ABEF8[i].flag1EA != 0 && D_800F4C68 == 0) {
                func_800225DC(n == 1 ? 0 : i + 1);
            }
            func_8006ECE8(i);
            func_800735D8(0);
            func_800747EC(0);
            func_80036FD4(i + 1);
            if (n == 1) {
                func_80070734(D_800ABEB0);
            }
            if (D_800FF6B0 == 0 && n == 1) {
                D_800ADAF0[1].enable18 = n;
                D_800ADAF0[0].enable18 = n;
                SCRATCH_CURSOR = func_8001FC28(frame + 0xAF8, SCRATCH_CURSOR, 0, D_80107190, 320,
                                               256 - (D_80107190 << n));
                if (i == 0) {
                    if ((D_80107190 -= 16) < 0) {
                        D_80107190 = 0;
                        D_800ADAF0[1].enable18 = 0;
                        D_800ADAF0[0].enable18 = 0;
                    }
                }
            }
            func_8005007C(D_800F4C70[i], D_800F2AB4[i], i);
            if (i == 0) {
                ovl667_func_801151E0();
            }
        }
    }

    if (D_800FF6B0 >= 4) {
        func_80036030();
        if (D_800FF6B0 == 6) {
            func_80036068();
        }
    }
    if (ovl667_D_80118ED4 == 0) {
        ovl667_func_801153F0();
    }
}
