/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"

typedef struct { short p0, p2, p4, p6, w8; unsigned short wA; short pC; unsigned short wE; } W5A;

extern void func_80097704(), func_80097714();
extern W5A D_801146C0;
extern W5A *D_800ABD18;
extern char D_800ABD20[];
extern volatile int *D_800ABD30;
extern int D_801146E8[3];
extern int D_801146D8[3];
extern volatile int D_801146D0;

int func_8009B758(void);
s32 func_8009B84C(void);
s32 func_8009B854(void);

int func_8009B758(void) {
    W5A *a = &D_801146C0;
    volatile W5A *b = D_800ABD18;
    char *s;
    int r;
    int t;
    volatile int *d;
    volatile int *e;
    a->wA = 5;
    a->w8 = 0xCE;
    a->wE = 0xD8;
    b->wA = 0x50;
    b->w8 = a->w8;
    b->wE = a->wE;
    b->wA |= 0x10;
    ((W5A *)b)->wA = a->wA;
    s = D_800ABD20;
    func_80097714(3, s);
    func_80097704(3, s);
    D_800ABD30[1] |= 0x100;
    e = D_801146E8;
    d = D_801146D8;
    e[0] = 0;
    d[0] = e[0];
    e[1] = 0;
    d[1] = e[1];
    e[2] = 0;
    t = e[2];
    D_801146D0 = 0;
    /* ASM_BARRIER: return constant 0 through a register (original li v0,0 after the last store) */
    __asm__ volatile("" : "=r"(r) : "0"(0));
    d[2] = t;
    return r;
}

s32 func_8009B84C(void) {
    return 0;
}

s32 func_8009B854(void) {
    return 0;
}
