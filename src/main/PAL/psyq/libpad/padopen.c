/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libpad/padpoll.h"
#include "psyq/libpad/padwait.h"
#include "psyq/libpad/padenab.h"
#include "psyq/libpad/padopen.h"
struct HW { unsigned char b0; char p1[3]; volatile unsigned short s4; short p6; volatile short s8; volatile unsigned short sA; char pC[2]; volatile short sE; };

struct A { char p0[0xC]; char *f0C; char p1[0x27]; unsigned char b37; char p2[0x18]; unsigned char b50; char p3[0x97]; unsigned char bE8; };

struct W { int x; };

extern int func_80096F04();
extern int func_80096F94();
extern int func_80097714();
extern int func_80096F14();
extern struct HW *D_800ABCCC;
extern volatile int *D_800ABCC8;
extern int D_800ABCA8;
extern struct W D_800ABCC0[];
extern void (*D_800ABC88)();
extern void (*D_800ABC8C)();

int func_80098154();

void func_80098110() { func_80096F04(); func_80096F94(3, 1); func_80097714(2, &D_80114418); func_80096F14(); }

int func_80098154(a0) struct A *a0; {
    struct W *p;
    struct HW *hw;
    int v;
    int t;
    int cur;
    D_800ABCCC->sA = 0x40;
    D_800ABCCC->sA = 0;
    D_800ABCCC->s8 = 0xD;
    D_800ABCCC->sE = 0x88;
    func_8009A7F4(a0->bE8 == 8 ? 0x50 : 0x91);
    hw = D_800ABCCC;
    t = 0x1003;
    if (D_800ABCA8 != 0) t = 0x3003;
    hw->sA = t;
    if ((cur = (&D_800ABCC0[D_800ABCA8])->x) >= 0) {
        struct W *base;
        if (cur > 0) {
            base = D_800ABCC0;
        lp:
            v = base[D_800ABCA8].x - 1;
            base[D_800ABCA8].x = v;
            D_800ABC88(a0->f0C + v * 0xF0);
            if (base[D_800ABCA8].x > 0) goto lp;
        }
        p = &D_800ABCC0[D_800ABCA8];
        if (p->x == 0) {
            p->x = -1;
            D_800ABC88(a0);
            D_800ABC8C(a0);
        }
    }
    if (D_800ABCCC->s4 & 0x200) {
        D_800ABCCC->sA |= 0x10;
        if (D_800ABCCC->s4 & 0x200) {
            while (func_8009A814() == 0);
            D_800ABCCC->b0 = 1;
            func_8009A7F4(0x64);
            if (func_80098A00() == 0) return 0;
            func_80098A90();
            *(volatile unsigned char *)&D_800ABCCC->b0;
            func_8009A7F4(0x1AE);
            goto l1;
        l1b:
            if (func_8009A814() != 0) return 0;
        l1:
            if (!(*D_800ABCC8 & 0x80)) goto l1b;
            D_800ABCCC->b0 = 0x42;
            func_8009A7F4(0x3C);
            if (func_80098A00() == 0) return 0;
            func_80098A90();
            *(volatile unsigned char *)&D_800ABCCC->b0;
            func_8009A7F4(0x1AE);
            goto l2;
        l2b:
            if (func_8009A814() != 0) return 0;
        l2:
            if (!(*D_800ABCC8 & 0x80)) goto l2b;
            D_800ABCCC->b0 = 1;
            func_8009A7F4(0x3C);
            if (func_80098A00() == 0) return 0;
            func_80098A90();
            *(volatile unsigned char *)&D_800ABCCC->b0;
            return 0;
        }
        *D_800ABCC8 = -0x81;
    }
    if (a0->b50 != 0 && a0->b37 != 0) return 0;
    return 1;
}
