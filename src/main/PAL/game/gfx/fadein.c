#include "game/gfx/fadectl.h"
#include "game/gfx/fadeset.h"
#include "game/gfx/fadein.h"
extern int D_800AC650[];

void func_8005C8D8(int a0) {
    if (D_800AC650[0x41C / 4] == 0) {
        func_8005C7A4();
    }
    D_800AC650[0x438 / 4] = a0 + 2;
    D_800AC650[0x434 / 4] = 1;
    D_800AC650[0x43C / 4] = 0;
    if (D_800AC650[0x408 / 4] != 1) {
        if (D_800AC650[0x408 / 4] == 0) {
            if (D_800AC650[0x3EC / 4] == 0xFF) {
                goto b5;
            }
        }
    } else {
b5:
        D_800AC650[0x408 / 4] = 2;
        D_800AC650[0x3E0 / 4] = 1;
        func_80054D30(0x20, 0, 0x100);
        D_800AC650[0x3F4 / 4] = 1;
    }
    D_800AC650[0x3F8 / 4] = 3;
    D_800AC650[0x41C / 4] = 2;
}
