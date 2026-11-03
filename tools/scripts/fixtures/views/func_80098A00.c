/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

/* legacy typing of shared globals in this part of the unit */
#define D_800ABCC8 ((struct Q_1 *)D_800ABCC8)

#define D_800ABCCC ((struct R *)D_800ABCCC)

#define func_8009A814 ((int (*)())func_8009A814)

#undef D_800ABCC8

#undef D_800ABCCC

#undef func_8009A814

struct HW { volatile unsigned char b0; char p1[3]; volatile unsigned short s4; short p6; short s8; unsigned short sA; char pC[2]; volatile short sE; };

struct Q { int a; };

extern int func_8009A7F4(), func_8009A814();

extern struct HW *D_800ABCCC;

extern struct Q *D_800ABCC8;

int func_80098A00() {
    D_800ABCC8->a = -0x81;
    if (D_800ABCCC->c & 0x80) {
        do {
            if (func_8009A814()) return 0;
        } while (D_800ABCCC->c & 0x80);
    }
    D_800ABCCC->f |= 0x10;
    return 1;
}
