/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/menu_transition.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDispMask ((int (*)())SetDispMask)

#define func_8005CA88 ((int (*)())func_8005CA88)

#define D_800F4D70 (*(int (*))&D_800F4D70)

#define D_800F4C6A (*(short (*))&D_800F4C6A)

#undef SetDispMask

#undef func_8005CA88

#undef D_800F4D70

#undef D_800F4C6A

void func_8003A544(void) {
    s32 next_frame;
    s32 mode;
    s32 status;
    s8 next_step;
    s8 opcode;
    s32 step;

    next_frame = (s32)((u32)D_800F2DE4 + 1U);
    D_800F2DE4 = next_frame;
    if (next_frame == 2) {
        SetDispMask(1);
    }
    if (D_800F4D44 == 4) {
        func_80035024(1);
    }
    mode=D_800FFC10;
    if (mode == 1) {
        status = func_80059E40(D_800F2DE4);
    } else {
        if (mode == 0) {
            status = func_8005CA88();
        } else {
            status = 1;
        }
    }
    step = D_800F3A2C;
    if (step >= 0) {
        if (D_8009F840[D_8009F8D0[D_800F4BE8][step]]() == 1) {
            D_800F4D7C = 0;
            next_step = (u8) D_800F3A2C + 1;
            D_800F3A2C = next_step;
            opcode = D_8009F8D0[D_800F4BE8][next_step];
            if (opcode < 0) {
                D_800F4BE8 = (u16) -opcode;
                D_800F3A2C = -1;
                if (D_800FFC10 == 0) {
                    func_8005CA14();
                }
            }
        }
    } else {
        if((status==1 && D_800FFC10==status) || (D_800FFC10==0 && status==2) || (D_800FFC10>=2 && status==1)) {
            D_8009F870[D_800F4BE8]();
            D_800F4D8A = 0;
            D_800F4C6A = (u16) ((u16) D_800AD50A >> 8);
            D_800F3A26 = D_800AD50A & 0xFF;
        }
    }
    if (D_800F4C6A != 1) {
        *(void **)0x1F800000 = func_8001FDD4((u32 *)((u8 *)D_800ACC14 + 0xB6C), *(void **)0x1F800000, 0, 0, 0x140, 0x200, 0, 0, 0);
    }
}
