/* RODATA 0x80011570 0x18 */
#include "game/data/cardini.h"
#include "game/data/savedat.h"
#include "game/data/savebld.h"
#include "game/system/modeinit.h"
typedef struct {
    char b[0x4000];
} Struct4000;

typedef struct {
    char b[0x2C];
} Struct2C;

extern int func_80096434();

extern Struct4000 *D_800102F0;
extern char D_800A40A8[][0x40];
extern char D_800A4128[][0x40];
extern unsigned char D_800A41E8;
extern unsigned char D_800A41E9 __asm__("D_800A41E8+1");
extern short D_800A41EA __asm__("D_800A41E8+2");
extern char D_800A45C8[];
extern char D_800A4648[][0x80];
extern Struct4000 *D_800ACC0C;
extern Struct2C D_800AD4D8;
extern Struct2C D_800F2E30[3];
extern short D_800F3AF8;
extern short D_800F4D3C;
extern char D_800F4D50[];
extern short D_800FFC18;
extern Struct2C D_8010B258[3];
extern int D_801272F0[];

int func_8007A310(a0, a1)
short a0;
int a1;
{
    int i;
    int r;
    Struct4000 *p;

    if (a0 > 0) {
        D_800A41E8 = 0;
    }
    switch (D_800A41E8) {
    case 0:
        D_800A41EA = 0;
        func_80096434(D_800ACC0C, 0x1AA8);
        i = D_800F3970 != 0;
        if (a0 == 1) {
            D_800A41E9 = 1;
            if (D_800F3AF8 < 0x10) {
                func_80079FF8(D_800ACC0C, D_800A40A8[i], 0x250, 0x1FC, 0x254, 0x1D8, D_800A45C8);
                func_80079200(D_800F3AF8);
            } else if (D_800F3AF8 & 0x10) {
                func_80079FF8(D_800ACC0C, D_800A4128[D_800F4D3C], 0x250, 0x1FC, 0x254, 0x1D8, D_800A4648[D_800F4D3C]);
                func_80079200(D_800F3AF8);
            } else if (D_800F3AF8 == 0x20) {
                *D_800ACC0C = *D_800102F0;
            }
        } else if (a0 >= 2) {
            D_800A41E9 = 0;
            if (D_800F3AF8 < 0x10) {
                func_80079FF8(D_800ACC0C, D_800A40A8[i], 0x250, 0x1FC, 0x254, 0x1D8, D_800A45C8);
                func_80079200(D_800F3AF8);
            } else if (D_800F3AF8 & 0x10) {
                func_80079FF8(D_800ACC0C, D_800A4128[D_800F4D3C], 0x250, 0x1FC, 0x254, 0x1D8, D_800A4648[D_800F4D3C]);
                func_80079200(D_800F3AF8);
            } else if (D_800F3AF8 == 0x20) {
                *D_800ACC0C = *D_800102F0;
            }
        }
        D_800A41E8 = 1;
        break;
    case 1:
        D_800A41E8 = 2;
        break;
    case 2:
        for (i = 0x1D; i >= 0; i--) {
        }
        if (D_800F3AF8 == 0x20) {
            r = func_80078610((short)a1, 0, D_800F4D50, D_800ACC0C, 2, D_800A41E9);
        } else {
            r = func_80078610((short)a1, 0, D_800F4D50, D_800ACC0C, 1, D_800A41E9);
        }
        if (r == -1) {
            if (D_800A41EA < 3) {
                D_800A41EA = (unsigned short)D_800A41EA + 1;
                break;
            }
            D_800A41E8 = 5;
        } else {
            D_800A41E8 = 3;
        }
        break;
    case 3:
        i = func_80078178();
        if (i != 0) {
            if (i == 1) {
                D_800A41E8 = 4;
                switch (D_800FFC18) {
                case 3:
                    D_8010B258[D_800F4D3C] = D_800AD4D8;
                    for (i = 0; i < 3; i++) {
                        D_800F2E30[i] = D_8010B258[i];
                    }
                    break;
                case 6:
                case 7:
                    p = D_800ACC0C;
                    D_801272F0[D_800F4D3C] = *(int *)((char *)p + 0x280);
                    break;
                }
            } else {
                if (D_800A41EA >= 3) {
                    D_800A41E8 = 5;
                } else {
                    D_800A41E8 = 1;
                    D_800A41EA = (unsigned short)D_800A41EA + 1;
                }
            }
        }
        break;
    case 4:
    case 5:
        break;
    }
    return D_800A41E8;
}
