/* RODATA 0x80011270 0x2c */
#include "types.h"
#include "scratchpad.h"
#include "rr/line_emit.h"
#include "m2c_macros.h"
#include "game/effect/effect.h"
#include "game/effect/fxspr.h"
#include "game/gfx/fadesel.h"
#include "game/gfx/fadeset.h"
#include "game/gfx/fadespr.h"
#include "game/sound/sndvab.h"
#include "rr/driver_control.h"
#include "rr/bios_devices.h"
#include "rr/course_choices.h"
#include "game/gfx/drawbuf.h"

struct Entry {
    int frame;
    int cmd;
};

struct Buf {
    char pad[0x19];
    unsigned char r;
    unsigned char g;
    unsigned char b;
    char pad2[0x22778 - 0x1C];
};

extern struct Entry D_800A20CC[];
extern int D_800ABEAC;
extern struct Buf D_800ADAF0[];
extern struct {
    char pad0[0x3E0];
    int f3E0;
    int f3E4;
    int f3E8;
    int f3EC;
    unsigned char f3F0;
    unsigned char f3F1;
    unsigned char f3F2;
    unsigned char f3F3;
    int f3F4;
    int f3F8;
    int f3FC;
    int f400;
    int f404;
    int f408;
    int f40C;
    short f410;
    short f412;
    short f414;
    short f416;
    short f418;
    short f41A;
    int f41C;
    int f420;
    int f424;
    int f428;
    int f42C;
    int f430;
    int f434;
    int f438;
    int f43C;
    int f440;
} D_800AC650;
extern void func_8005C37C(int, int, int, int);
extern int rsin(int);
extern int func_8001FDD4(char *, int, int, int, int, int, int, int, int);
extern int func_8004E008();
extern s32 D_800102F0;
extern M2C_UNK D_800ACA94;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;

int func_8005CA88(void);
void func_8005D2A0(void);

int func_8005CA88(void) {
    short sp28[4];
    unsigned char sp30[4];
    int i;
    int v;
    int cmd;
    int ret = 0;

    cmd = -1;
    for (i = 0; D_800A20CC[i].frame >= 0; i++) {
        if (D_800A20CC[i].frame == D_800AC650.f3E4) {
            cmd = D_800A20CC[i].cmd;
            break;
        }
    }

    switch (cmd) {
    case 0:
        D_800AC650.f3F4 = 1;
        break;
    case 1:
        func_80054D30(0x20, 0x100, 0);
        break;
    case 2:
        D_800AC650.f3F8 = 1;
        break;
    case 3:
        D_800AC650.f3F8 = 2;
        D_800AC650.f3FC = 0;
        break;
    case 4:
        D_800AC650.f408 = 1;
        break;
    case 5:
        D_800AC650.f408 = 2;
        break;
    case 6:
        D_800AC650.f410 = 1;
        break;
    case 7:
        D_800AC650.f418 = 1;
        break;
    case 8:
        D_800AC650.f414 = 1;
        D_800AC650.f41C = 1;
        D_800AC650.f424 = 0;
        break;
    case 9:
        D_800AC650.f434 = 1;
        D_800AC650.f43C = 0;
        D_800AC650.f438 = 0;
        break;
    case 10:
        D_800AC650.f434 = 2;
        D_800AC650.f43C = 0;
        break;
    }

    if (D_800AC650.f428 == 1) {
        D_800AC650.f3E8 -= 0x10;
        if (D_800AC650.f3E8 <= 0x80) {
            D_800AC650.f3E8 = 0x80;
            D_800AC650.f428 = 2;
        }
    }

    switch (D_800AC650.f434) {
    case 1:
        D_800AC650.f43C++;
        D_800AC650.f440 = (D_800AC650.f43C * 0xFF) / 16;
        if (D_800AC650.f43C >= 0x10) {
            D_800AC650.f434 = 0;
            D_800AC650.f440 = 0xFF;
        }
        break;
    case 2:
        D_800AC650.f43C++;
        D_800AC650.f440 = ((0x10 - D_800AC650.f43C) * 0xFF) / 16;
        if (D_800AC650.f43C >= 0x10) {
            D_800AC650.f434 = 0;
            D_800AC650.f440 = 0;
        }
        break;
    }

    if (D_800AC650.f440 > 0) {
        func_8005C4E0();
    }

    if (D_800AC650.f3F4) {
        func_80054D50();
    }

    switch (D_800AC650.f3F8) {
    case 1:
        D_800AC650.f404 += 8;
        if (D_800AC650.f404 >= 0x100) {
            D_800AC650.f404 = 0x100;
            D_800AC650.f3F8 = 0;
        }
        break;
    case 2:
        D_800AC650.f3FC++;
        if (D_800AC650.f3FC >= 0x10) {
            D_800AC650.f3F8 = 0;
            D_800AC650.f3FC = 0x10;
            if (D_800AC650.f430 == 0) {
                func_8004F274(1);
                D_800AC650.f430 = 1;
            }
        }
        D_800AC650.f400 = 0xCE - (D_800AC650.f3FC * 0x42) / 16;
        break;
    case 3:
        D_800AC650.f404 -= 8;
        if (D_800AC650.f404 <= 0) {
            D_800AC650.f404 = 0;
            D_800AC650.f3F8 = 0;
        }
        break;
    }

    func_8005C37C(0, 0x2C, D_800AC650.f400, D_800AC650.f404);

    if (D_800AC650.f410) {
        int c;
        D_800AC650.f412++;
        if (D_800AC650.f412 > 0x10) {
            D_800AC650.f412 = 0x10;
        }
        c = 0xFF - D_800AC650.f3EC;
        c *= D_800AC650.f404;
        if (c < 0) {
            c += 0xFF;
        }
        c >>= 8;
        c *= D_800AC650.f3E8;
        if (c < 0) {
            c += 0xFF;
        }
        c >>= 8;
        sp30[0] = sp30[1] = sp30[2] = c;
        sp28[0] = 0x10;
        sp28[2] = D_800AC650.f412 * 18 + 0x10;
        for (i = 0; i < 2; i++) {
            sp28[1] = sp28[3] = i * 40 + 0x172;
            func_8004D290(sp28, sp30, 0, 10);
            sp28[1] = sp28[3] = i * 40 + 0x173;
            func_8004D290(sp28, sp30, 0, 10);
        }
    }

    if (D_800AC650.f418) {
        int v2;
        D_800AC650.f41A++;
        if (D_800AC650.f41A > 0x10) {
            D_800AC650.f41A = 0x10;
            v2 = 0x100;
        } else {
            v2 = D_800AC650.f41A * 16;
        }
        v2 = (v2 * D_800AC650.f404) / 256;
        func_8005C37C(3, 0x42, 0x17C, v2);
    }

    if (D_800AC650.f414) {
        D_800AC650.f416++;
        if (D_800AC650.f416 > 0x10) {
            D_800AC650.f416 = 0x10;
            v = 0x100;
        } else {
            v = D_800AC650.f416 * 16;
        }
        v = (v * D_800AC650.f404) / 256;
        func_8005C37C(2, 0x68, 0x13C, v);
        if (D_800AC650.f428 == 0) {
            v = (rsin((D_800ABEAC << 7) & 0xF80) * v) / 4096;
        }
        func_8005C37C(1, 0x68, 0x13C, v);
    }

    switch (D_800AC650.f408) {
    case 1:
        D_800AC650.f3EC += 8;
        if (D_800AC650.f3EC >= 0xFF) {
            D_800AC650.f3EC = 0xFF;
            D_800AC650.f408 = 0;
            D_800AC650.f3F4 = 0;
        }
        break;
    case 2:
        D_800AC650.f3EC -= 8;
        if (D_800AC650.f3EC <= 0) {
            D_800AC650.f3EC = 0;
            D_800AC650.f408 = 0;
        }
        break;
    }

    func_8005C26C(D_800AC650.f3EC);
    *(int *)SCRATCHPAD = func_8001FDD4(D_800ACC14 + 0xB6C, *(int *)SCRATCHPAD, 0, 0, 0x140, 0x200,
                                       D_800AC650.f3F0, D_800AC650.f3F1, D_800AC650.f3F2);
    for (i = 0; i < 2; i++) {
        D_800ADAF0[i].r = D_800AC650.f3F0;
        D_800ADAF0[i].g = D_800AC650.f3F1;
        D_800ADAF0[i].b = D_800AC650.f3F2;
    }

    D_800AC650.f3E4++;
    if (D_800AC650.f3E4 > 3000) {
        D_800AC650.f3E4 = 3000;
    }

    if (D_800AC650.f41C == 1) {
        D_800AC650.f424++;
        if (D_800AC650.f424 >= 1000) {
            if (D_800AC650.f3E0 == 0) {
                D_800AC650.f408 = 2;
                D_800AC650.f3E0 = 1;
                func_80054D30(0x20, 0, 0x100);
                D_800AC650.f3F4 = 1;
            } else {
                D_800AC650.f408 = 1;
                D_800AC650.f3E0 = 0;
                func_80054D30(0x20, 0x100, 0);
            }
            D_800AC650.f424 = 0;
        }
        ret = 1;
    } else if (D_800AC650.f41C == 3) {
        D_800AC650.f420++;
        if (D_800AC650.f420 > 20) {
            D_800AC650.f420 = 20;
            ret = 2;
        } else {
            ret = 1;
        }
    }

    return ret;
}

void func_8005D2A0(void) {
    SetDispMask(0);
    func_80096434(&D_800ACA94, 0x78);
    func_8005C13C(0);
    func_8005C988(0);
    D_800ACC18 = D_800102F0;
    func_8004E008();
    D_800F4C6A = 2;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
}
