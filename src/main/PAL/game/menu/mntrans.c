/* RODATA 0x80010774 0x4c */
#include "rr/menu_transition.h"

extern int func_8001ECE8();
extern int func_80036068();
extern int func_8002A604();
extern int func_8003A1B4();
extern int func_8003A198();
extern int func_80059314();
extern int D_800AC5DC;
extern int D_800F4D40;
extern int D_800ACC18;
extern int D_800F4D70;
extern int D_800102F0;
extern signed char D_800AD4D8[];
extern short D_800F3826;

void func_8003A2D4();
void func_8003A544(void);

/* legacy typing of shared globals in this part of the unit */
#define func_80092CB8 ((int (*)())func_80092CB8)
#define func_8005CA88 ((int (*)())func_8005CA88)
#define D_800F4C6A (*(short (*))&D_800F4C6A)

void func_8003A2D4() {
    int a0, a1, a3;
    D_800F2DE4 = 0;
    D_800AC5DC = 0x80;
    D_800ACC18 = D_800F4D40;
    a3 = 0;
    switch (D_800F4BE8) {
    case 0:
        func_8001ECE8(0, 0, 0, 1);
        D_800FFC10 = 1;
        D_800ACC18 = D_800F4D70;
        goto l428;
    case 10:
        if (D_800F4D44 == 5) goto five;
        func_8001ECE8(0, 0, 0, 1);
        D_800FFC10 = 1;
        D_800ACC18 = D_800102F0;
        goto l428;
    five:
        func_8001ECE8(0, 0, 0, 2);
        D_800FFC10 = 0;
        goto l428;
    case 6:
        func_80092CB8(0);
        func_80036068();
        a3 = 1;
    case 1:
    case 4:
        func_8001ECE8(8, 8, 8, a3 | 2);
        D_800FFC10 = 2;
        goto l418;
    default:
        func_8001ECE8(0, 0, 0, 2);
        D_800FFC10 = 0;
    }
l418:
    func_8002A604();
    func_8003A1B4();
l428:
    func_8003A198();
    if (D_800FFC10 == 1) {
        func_80092CB8(0);
        a1 = 0;
        switch (D_800F4D44) {
        case 0: a0 = 8; break;
        case 1:
            a0 = D_800AD4D8[0x17];
            a1 = 3 - D_800AD4D8[a0 + 8];
            break;
        case 2:
        case 3: a0 = 9; break;
        case 4: a0 = 10; break;
        case 6: a0 = D_800F3826 + 0xB; break;
        default:
            a0 = 0;
            a1 = a0;
            D_800FFC10 = 3;
            break;
        }
        if (D_800FFC10 == 1) func_80059314(a0, a1);
    } else if (D_800FFC10 == 0) {
        func_8005CA88();
    }
    D_800F4C6A = 1;
    D_800F3A26 = 0x24;
    D_800F4D8A = 0;
}

#undef func_80092CB8
#undef func_8005CA88
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
        func_80092CB8(1);
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
