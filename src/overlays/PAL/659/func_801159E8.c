#include "rr/overlay659_preview_init.h"

void ovl659_func_801159E8(void) {
    s32 index;
    s32 *scratch;

    if (D_800F4D8A == 0) {
        D_800F2DE4 = 0;
        D_800ABEB0 = 0;
        D_800AC64A = 0;
        D_800AC5E0 = 3;
        func_80053CB0();
        func_8003A198();
        func_8002A604();
        func_8003A1B4();
        D_800ACC18 = D_800F4D74;
        func_8003E928(0, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x54);
        func_8003E928(1, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 1, 0xC);
        func_8003E928(2, 0xC, 0x90, 0x44, 0x20, 0, 0, 1, 0, 0x1D);
        func_8003E928(3, 0xC, 0x170, 0xD0, 0x20, 0, 0, 8, 4, (s32) D_800AD4D8.phase17);
        func_8003E928(4, 0x74, 0x58, 0x1C, 0x20, 0, 0, 1, 0, 0x37);
        func_8003E928(5, 0x74, 0x80, 0x1C, 0x20, 0, 0, 1, 0, 0x38);
        func_8003E928(6, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
        index = 0;
        do {
            if (index == D_800AD4D8.phase17) {
                D_800F4D80[3].cells[index] = 2;
            } else {
                D_800F4D80[3].cells[index] = 0;
            }
            index += 1;
        } while (index < 8);
        scratch = RR_MENU_PREVIEW_SCRATCH_WORDS;
        scratch[6] = 0x200;
        scratch[7] = 0;
        scratch[8] = 0;
        scratch[2] = 0;
        scratch[3] = -0x1A0;
        scratch[4] = 0;
        D_800ABEF8[1].pos20[0] = 0;
        D_800ABEF8[1].pos20[1] = 0;
        D_800ABEF8[1].pos20[2] = 0;
        func_8002E4F4(D_800ABEF8);
        D_800ABEF8[0].pos20[1] = 0xA;
        D_800ABEF8[0].pos20[2] = 0x134;
        D_800ABEF8[0].rot54 = 0xA00;
        D_800F4D8A = 1;
    } else if (D_800F4D8A == 1) {
        if (func_8003BAB0() == 1) {
            D_800F4D8A = 2;
        }
    } else if (D_800F4D8A == 2) {
        func_80052800();
        func_800528A8(0, 2);
        D_800F4C6A = 1;
        D_800F3A26 = 0x12;
        D_800F4D8A = 0;
    }
    func_8002A344(0x1002E);
}
