#include "psyq/libsnd/utrev3.h"
typedef struct { char a, b, c, d; } W;
void func_80080C74(short a0, short a1, short a2, W a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12) {
    int x0 = a0;
    int x1 = a1;
    int x2 = a2;
    register int v asm("$17") = a12; /* ASM_BARRIER: pin s1 to match original reg */
    func_800850C4(x0, x1, x2, &a3);
    a3.b = v;
    func_800867E4(x0, x1, x2, &a3);
    v = v & 0xFF;
    if (v == 0) func_800867A4();
    else if (v == 4) func_800867C4();
}
