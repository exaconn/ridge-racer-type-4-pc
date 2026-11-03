/* RODATA 0x80010910 0x14 */
#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    s16 s4;
    s16 s6;
    s16 s8;
    s16 sA;
    s8 c0C[8];
    u8 b14;
    u8 b15;
    u8 b16;
    u8 b17;
    s16 s18;
    s16 s1A;
    s16 s1C;
    s16 s1E;
    s16 s20;
    u8 pad22[2];
} UiElem24;

extern s32 D_800ACC14;
extern UiElem24 *D_800F4D80;

extern s32 func_8001FD20();
extern s32 func_80020074();
extern void func_8003F920();
extern s32 func_80090424();

void func_8003EA04(s32 arg0, s32 arg1, s32 arg2) {
    s32 pad[8];
    s32 s0;
    s32 s1;
    s32 s3;
    s32 s5;
    /* ASM_BARRIER: Allocate s6 to $s6 so s5 gets $s5 in global-alloc */
    register s32 s6 asm("s6");
    s32 a0;
    s32 v;
    s32 t;
    s32 b;
    s32 d;
    s32 c;
    UiElem24 *p;

    if (D_800F4D80[arg0].b2 == 0) {
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x70, *(s32 *)0x1F800000, arg1, arg2, 1, D_800F4D80[arg0].sA, 0xFF, 0xFF, 0xFF);
    } else {
        s1 = 0;
        s3 = *(s32 *)0x1F800000;
        s5 = ((D_800F4D80[arg0].b17 - 0x80) << 4) & 0xFFF;
        if ((b = D_800F4D80[arg0].b2) != 0) {
            do {
                switch (D_800F4D80[arg0].c0C[s1]) {
                default:
                case 0:
                    a0 = 0xC00;
                    s0 = 0;
                    break;
                case 2:
                    a0 = s5;
                    s0 = 0;
                    break;
                case 1:
                    a0 = 0xC00;
                    s0 = 3;
                    break;
                case 3:
                case 4:
                    a0 = s5;
                    s0 = 3;
                    break;
                }
                s6 = arg1 + s1 * 10 + 4;
                c = 0x1E;
                v = func_80090424(a0);
                t = s0 + 1;
                s0 = ((t + v / 0x801) << 3) + 0xD0;
                s3 = func_80020074(D_800ACC14 + 0x70, s3, s6, arg2 + 8, 8, 0x10, s0, 0x10, c, 0x7F3A, 0, 0x80);
                s1++;
            } while (s1 < D_800F4D80[arg0].b2);
        }
        D_800F4D80[arg0].b17 += 8;
        p = D_800F4D80;
        /* ASM_BARRIER: Prevent sched1/sched2 from hoisting reloads above b17 += 8 store */
        __asm__ volatile("");
        d = arg1 - 12;
        b = p[arg0].b17;
        /* ASM_BARRIER: Keep unused b17 load live and reload D_800F4D80 without a branch basic-block boundary */
        __asm__("" : "=m"(D_800F4D80) : "r"(b), "r"(d));
        *(s32 *)0x1F800000 = s3;
        func_8003F920(arg1, arg2, s6 - d, D_800F4D80[arg0].sA, 0xCE, 0xCE, 0xCE);
    }
}
