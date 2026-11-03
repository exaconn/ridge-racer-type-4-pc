/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libspu/spuregs2.h"
#include "psyq/libspu/spuregs2.h"
extern int func_8007EAC4();
extern int func_8007F3D0();
extern int func_8007F6EC();
extern int func_8008B364();
extern int D_800A47F4;
extern int D_800A4844;
extern int D_800A4854;
extern struct { int a; short b, c; int d, e; } D_800A485C;
extern int D_800A4870; extern int D_800A486C;
extern unsigned short D_800A48A2;
extern int D_800A48A4; extern int D_800A48B8;
extern int D_800A48BC;
extern int D_800A48C0;
extern int D_800A48C8;
void func_8007F604(a0) int a0; {
    unsigned short *p;
    int i; unsigned c; struct { int a; short b, c; int d, e; } *q;
    func_8008B364();
    func_8007EAC4(a0);
    c = 0xC000;
    if (a0 == 0) {
        i = 0x17;
        p = &D_800A48A2;
        do {
            *p = c;
            i--;
            p--;
        } while (i >= 0);
    }
    func_8007F6EC(c);
    D_800A484C = 0;
    D_800A4850 = 0;
    q = &D_800A485C;
    q->a = 0;
    q->b = 0;
    q->c = 0;
    q->d = 0;
    q->e = 0;
    D_800A4854 = D_800A48C8;
    func_8007F3D0(0xD1, D_800A48C8, 0);
    D_800A48B8 = 0;
    D_800A48BC = 0;
    D_800A48C0 = 0;
    D_800A4848 = 0;
    D_800A47F4 = 0;
    D_800A4844 = 0;
    D_800A4870 = 0;
    D_800A486C = 0;
    D_800A48A4 = 0;
    /* ASM_BARRIER: keeps epilogue stack loads (lw ra/s*) after the final global stores; sched2 otherwise hoists/sinks them */
    __asm__ volatile ("" : : : "memory");
}
