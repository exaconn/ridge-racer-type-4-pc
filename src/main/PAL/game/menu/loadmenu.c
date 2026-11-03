/* RODATA 0x80011588 0x1c */
#include "types.h"
#include "rr/mode_menu_state.h"
#include "m2c_macros.h"
#include "game/data/cardini.h"
#include "game/menu/menustep.h"
#include "game/data/cardini.h"
#include "game/data/savedat.h"
#include "game/menu/loadmenu.h"

extern short D_800A41EC;
extern short D_800A41EE;
extern int D_800ACC0C;
extern int func_8003D6F0();

void func_8007AB8C(int arg0);
void func_8007B3A0(void);

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D50 (*(char (*)[])&D_800F4D50)

int func_8007A9CC(a0, a1)
short a0;
int a1;
{
    int i;
    int r;
    int q;
    if (a0 > 0) {
        D_800A41EC = 0;
    }
    switch (D_800A41EC) {
    case 0:
        D_800A41EE = 0;
        D_800A41EC = 1;
        break;
    case 1:
        D_800A41EC = 2;
        break;
    case 2:
        for (i = 0x1D; i >= 0; i--) {
        }
        if (D_800F3AF8 != 0x10) {
            r = func_80078490((short)a1, 0, D_800F4D50, D_800ACC0C, 1);
        } else {
            r = func_80078544((short)a1, 0, D_800F4D50, D_800ACC0C, 0x80, 0x280);
        }
        if (r == -1) {
            if (D_800A41EE < 3) {
                D_800A41EE = (unsigned short)D_800A41EE + 1;
                break;
            }
            D_800A41EC = 5;
        } else {
            D_800A41EC = 3;
        }
        break;
    case 3:
        q = func_80078178();
        if (q != 0) {
            if (q == 1) {
                if (func_800797E0(D_800F3AF8) != 0) {
                    D_800A41EC = 4;
                } else {
                    D_800A41EC = 6;
                }
            } else {
                if (D_800A41EE >= 3) {
                    D_800A41EC = 5;
                } else {
                    D_800A41EC = 1;
                    D_800A41EE = (unsigned short)D_800A41EE + 1;
                }
            }
        }
        break;
    case 4:
    case 5:
    case 6:
        break;
    }
    return D_800A41EC;
}

#undef D_800F4D50

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A26 (*(short (*))&D_800F3A26)
#define D_800F4D8A (*(short (*))&D_800F4D8A)

void func_8007AB8C(int arg0) {
    if (arg0 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 3;
    } else {
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
    }
    D_800F4D8A = 0;
}

#undef D_800F3A26
#undef D_800F4D8A

void func_8007ABCC(void) {
    unsigned long clearAddress;
    u16 clockLow;
    s32 selection;
    u16 modeRange;
    s16 bank;
    s32 ready;
    s32 i;
    s32 defaultChoice;
    s32 layout;
    u16 phase;

    if (D_800F4D44 == 4) {
        func_80035024(2);
        if (D_800FF688 == -1) {
            func_8009AEAC(1, 1, 0);
        }
    }
    phase = D_800F4D8A;
    switch (phase) {
    case 0:
        func_80092CB8(0);
        func_8001ECE8(0x20, 0x20, 0x20, 2);
        D_800F2DE4 = 0;
        D_800F4D3C = 0;
        D_80109478[D_8010948E] = 3;
        /* ASM_BARRIER: finish the choice write before the clock read. */
        __asm__("" : : : "memory");
        __builtin_memcpy(&clockLow, &D_800AC5D4, sizeof(clockLow));
        bank = clockLow & 1;
        D_800AC594 = bank;
        D_800FFC00 = bank;
        func_8002A604();
        i = 23;
        /* ASM_BARRIER: retain the initialized byte counter before indexing. */
        __asm__("" : "+r"(i));
        clearAddress = (unsigned long)D_800F4D50;
        /* ASM_BARRIER: retain the real buffer address before its byte offset. */
        __asm__("" : "+r"(clearAddress));
        clearAddress += i;
        do {
            *(char *)clearAddress = 0;
            i--;
            clearAddress--;
        } while (i >= 0);
        func_8002B75C(D_800F4D50,D_800A457C);
        selection = D_800FFC18;
        /* ASM_BARRIER: keep the signed snapshot separate from the range read. */
        __asm__("" : "+r"(selection));
        modeRange = (u16)D_800FFC18;
        modeRange = (u16)(modeRange - 6);
        if (selection == 0) {
            func_8003E928(0x22, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x66);
            func_8003E928(0x23, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 0, 0x65);
            func_8003E928(0x24, 0xC, 0x90, 0x44, 0x20, 0, 0, 1, 0, 0x64);
            func_8003E928(0x25, 0xC, 0xB8, 0x44, 0x20, 0, 0, 1, 0, 0x55);
            D_800F3A1C = 0;
            D_800F3AF8 = 0;
            D_800A45B8 = 0x1001A;
            func_80053BB4(0);
        } else if (modeRange < 2U) {
            if (selection == 6) {
                func_8003E928(0x22, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x66);
                func_8003E928(0x23, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 0, 0x65);
                func_8003E928(0x24, 0xC, 0x90, 0x44, 0x20, 0, 0, 1, 0, 0x55);
                layout = 0x1001A;
            } else {
                func_8003E928(0x22, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x66);
                func_8003E928(0x23, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 0, 0x55);
                layout = 0x10012;
            }
            D_800A45B8 = layout;
            /* ASM_BARRIER: publish the layout before the badge status. */
            __asm__("" : : : "memory");
            D_800F3AF8 = 0x10;
            D_800F3A1C = 0x29;
            func_8003D228(1, 0x80, 0x58, 0xA8, 0x80, 0, 0xE02);
            D_800F3AFC[1].half02 = 1;
            D_801272F0[0] = -1;
            D_801272F0[1] = -1;
            D_801272F0[2] = -1;
        } else {
            if (selection == 1 || selection == 3 || selection == 5) {
                func_8003E928(0x22, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x66);
                func_8003E928(0x23, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 0, 0x55);
                D_800A45B8 = 0x10012;
                if (D_800FFC18 == 3) {
                    D_800F3A1C = 0x17;
                } else {
                    D_800F3A1C = 0x20;
                }
                func_8003D228(1, 0x80, 0x58, 0xA8, 0x80, 0, 0x702);
                D_800F3AFC[1].half02 = 1;
                D_800F3AF8 = (s16) (u16) D_800FFC18;
            } else if (selection == 2) {
                D_800A45B8 = 0x10016;
                func_8003E928(0x22, 0xC, 0x40, 0x44, 0x20, 0, 0, 1, 0, 0x65);
                func_8003E928(0x23, 0xC, 0x68, 0x44, 0x20, 0, 0, 1, 0, 0x55);
                D_800F3A1C = 0x21;
                D_800F3AF8 = selection;
            } else {
                D_800A45B8 = 0x1001A;
            }
        }
        func_8003E928(0x26, 0x80, 0xE0, 0x1C, 0x20, 0, 0, 1, 0, 0x68);
        func_8003E928(0x27, 0x80, 0x108, 0x1C, 0x20, 0, 0, 1, 0, 0x69);
        func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
        D_800F3AFC[0].half02 = 1;
        func_8003A198();
        defaultChoice = 3;
        /* ASM_BARRIER: keep the initialized fill value apart from the count. */
        __asm__("" : "+r"(defaultChoice));
        D_800F4CA8 = 3;
        D_8010948E = 0;
        for (i = 2; i >= 0; i--) {
            D_80109478[i] = defaultChoice;
        }
        D_8010948C = 0;
        for (i = 0; i < 3; i++) {
            D_8010B258[i] = D_800F2E30[i];
        }
        D_800F4D8A = 1;
        break;
    case 1:
        D_800F2DE4 = (s32)((u32)D_800F2DE4 + 1U);
        if (D_800F3A26 == phase) {
            func_80092CB8(1);
        }
        ready = func_8003B934();
        if (ready == phase) {
            func_80092CB8(1);
            D_800F4C6A = ready;
            D_800F3A26 = 0x48;
            D_800F4D8A = 0;
        }
        break;
    }
    func_8002A344(D_800A45B8);
    func_8002AF8C(0, 1, 1);
}

/* legacy typing of shared globals in this part of the unit */
#define func_8002A344 ((int (*)())func_8002A344)
#define func_8002AF8C ((int (*)())func_8002AF8C)
#define func_80035024 ((int (*)())func_80035024)
#define func_8009AEAC ((int (*)())func_8009AEAC)

void func_8007B3A0(void) {
    D_800F2DE4 += 1;
    if (D_800F4D44 == 4) {
        func_80035024(2);
        if (D_800FF688 == -1) {
            func_8009AEAC(1, 1, 0);
        }
    }
    func_8007B460();
    func_8003D6F0(0);
    if (D_800FFC18 != 3) {
        if (D_800FFC18 != 6) {
            if (D_800FFC18 == 7) {
                goto block_6;
            }
        } else {
            goto block_6;
        }
    } else {
block_6:
        func_8003D6F0(1);
    }
    func_8002A344(D_800A45B8);
    func_8002AF8C(1, 1, 1);
}

#undef func_8002A344
#undef func_8002AF8C
#undef func_80035024
#undef func_8009AEAC
