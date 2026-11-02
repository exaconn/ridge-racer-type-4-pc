#include "rr/menu_overlay_runtime.h"

void func_8002B75C(char *destination, char *source);
void func_800397BC(char *text);
s32 func_8004342C(s32 course, s32 selection, s32 car, s32 mode, s32 value, u8 *output);
/* The caller passes 0 in $a0; the callee ignores it. */
s32 ovl660_func_801156A8();
void ovl660_func_80114920(void);

extern char ovl660_D_801148C0[];
extern s32 ovl660_D_80117910;
extern s32 ovl660_D_80117914;
extern s16 ovl660_D_80117918;
extern char ovl660_D_80117920[12];
extern s32 ovl660_D_80117930;
extern s32 ovl660_D_80117938;
extern s32 D_800F4C64;
extern s32 D_800AD564;

void ovl660_func_8011572C(void) {
    s32 group;
    s32 base;
    s32 row;
    s32 i;
    s16 timer;
    s32 ready;

    D_800F2DE4++;
    switch (D_800F4D8A) {
    case 0:
        if (D_800F4C64 == -1) {
            group = 44;
            if (D_800FFC14[44].signedBytes[2] >= 2) {
                D_800F4D8A = 3;
            }
            break;
        }
        group = 0;
        if (D_800FFC14[1].signedBytes[3] >= 15) {
            func_8002AF8C(1, 1, 0);
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                ovl660_D_80117914 = 0;
                D_800F4D8A++;
                func_8002AF8C(0, 1, 0);
            }
        }
        base = 5;
        if (D_800F3914 == 7) {
            base = 3;
        }
        for (i = 0; i < D_800F3A30; i++) {
            row = base + i;
            func_8002B814((char *)&D_800FFC14[row].bytes[32], i + 1, 0);
            func_8002B934((char *)&D_800FFC14[row].bytes[35], D_800F4C80[i]);
            if (i == D_800ABEF8[0].steering288.finish20 && ovl660_D_80117910 < 5) {
                func_80041320(row, 1, -1);
                func_8002B75C((char *)&D_800FFC14[row].bytes[45], ovl660_D_801148C0);
            }
        }
        base = 9;
        if (D_800F3914 == 7) {
            base = 10;
        }
        if (ovl660_D_80117938 < 5) {
            func_80041320(base, 1, -1);
            func_8002B75C((char *)&D_800FFC14[base].bytes[45], ovl660_D_801148C0);
        }
        break;
    case 1:
        group = 11;
        if (ovl660_D_80117910 < 5) {
            func_8002B330(1, 4, (char *)&ovl660_D_80117914, 8, 0);
            func_800397BC(&ovl660_D_80117920[ovl660_D_80117914]);
            {
                s32 entry = ovl660_D_80117910 + 15;
                func_8002B75C((char *)&D_800FFC14[entry].bytes[42], ovl660_D_80117920);
            }
            func_8002B75C(D_800FF700[D_800AD522][D_800F3914][ovl660_D_80117910].name, ovl660_D_80117920);
            func_80041320(ovl660_D_80117910 + 15, 1, -((ovl660_D_80117914 + 15) << 8));
        }
        if (D_800FFC14[15].signedBytes[3] >= 15) {
            func_8002AF8C(1, 1, 0);
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                ovl660_D_80117914 = 0;
                D_800F4D8A++;
                func_8002AF8C(0, 1, 0);
            }
        }
        break;
    case 2:
        group = 22;
        if (ovl660_D_80117938 < 5) {
            func_8002B330(1, 4, (char *)&ovl660_D_80117914, 8, 0);
            func_800397BC(&ovl660_D_80117920[ovl660_D_80117914]);
            {
                s32 entry = ovl660_D_80117938 + 26;
                func_8002B75C((char *)&D_800FFC14[entry].bytes[42], ovl660_D_80117920);
            }
            func_8002B75C(D_800AD5C8[D_800AD522][D_800F3914][ovl660_D_80117938].name, ovl660_D_80117920);
            func_80041320(ovl660_D_80117938 + 26, 1, -((ovl660_D_80117914 + 15) << 8));
        }
        if (D_800FFC14[15].signedBytes[3] >= 15) {
            func_8002AF8C(1, 1, 0);
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                D_800F4D8A++;
                func_8002AF8C(0, 1, 0);
            }
        }
        break;
    case 3:
        group = 33;
        if (D_800FFC14[38].signedBytes[3] >= 17) {
            func_8002AF8C(1, 1, 0);
            if (ovl660_func_801156A8(0) >= 0) {
                func_8002AF8C(0, 1, 0);
                func_8003A198();
                func_8003A1B4();
                D_800AC5D0 = 64;
                D_800F4D8A = D_800AC64A + 4;
            }
        } else {
            func_8002AF8C(0, 1, 0);
        }
        if (D_800F4C64 != -1) {
            if (ovl660_D_80117930 == 1) {
                ready = func_8003708C((s8 *)&D_800F4D98);
                D_800FFC14[42].bytes[28] = 34;
                func_8004342C(D_800AC648, ready, D_800F4C64, ovl660_D_80117918, D_800AD564,
                              &D_800FFC14[42].bytes[29]);
                D_800FFC14[42].bytes[42] = 34;
                ovl660_D_80117930 = 2;
            } else if (ovl660_D_80117930 == 0 && (*(s32 *)&D_800F3A38[0] & 0x900000) == 0x900000) {
                ovl660_D_80117930 = 1;
            }
        }
        break;
    case 4:
        timer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (64 - timer) * 4, 69);
        group = 33;
        if (D_800AC5D0 <= 0) {
            D_800F4BE8 = 0;
            D_800F4C6A = 1;
            D_800F3A26 = 35;
            D_800F4D8A = 0;
        }
        break;
    case 5:
        timer = (u16)D_800AC5D0 - 2;
        D_800AC5D0 = timer;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (64 - timer) * 4, 69);
        group = 33;
        if (D_800AC5D0 <= 0) {
            group = -1;
            D_800F4D8A = 8;
        }
        break;
    case 6:
        timer = (u16)D_800AC5D0 - 1;
        D_800AC5D0 = timer;
        func_800207D4(&D_800ACC14->ordering.ordering70[0], (64 - timer) * 4, 69);
        group = -1;
        if (D_800AC5D0 <= 0) {
            D_800F4C6A = 1;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        }
        break;
    case 8:
        ready = func_8003B12C();
        if (ready == 1) {
            func_80053BB4(0);
            ovl660_func_80114920();
            D_800F4C6A = ready;
            D_800F3A26 = 50;
            D_800F4D8A = 0;
        }
        return;
    }
    func_8002A344(0x1000C);
    func_8003662C(D_800F2DE4);
    if (group >= 0) {
        i = 0;
        do {
            func_80040858(i + group);
            i++;
        } while (i < 11);
    }
    if ((u32)(D_800FFC14[0].bytes[2] - 3) < 5 || (u32)(D_800FFC14[44].bytes[2] - 3) < 5) {
        func_800411D4();
    }
}
