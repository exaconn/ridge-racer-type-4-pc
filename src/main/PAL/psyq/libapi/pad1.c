/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"
#include "psyq/libapi/pad2.h"

extern s32 D_800ABC50;
extern int func_80096F04();
extern int func_80096F14();
extern int func_80096F84();
extern int func_800976F4();
extern int func_8009774C();
extern int func_800977C4();
extern int func_800976C4();
extern int func_800976D4();
extern int func_80097724();
extern int func_800975E0(), func_800976E4(), func_80097738();
extern int func_80096F04(), func_80096F14(), func_80097704(), func_80097714();
extern int D_801143E8;
extern int D_801143F4;
extern int func_80097680();
extern int D_801143EC[];

void func_800973C4(s32 value);
s32 func_800973D0(void);
void func_800973E0();
void func_80097470();
s32 func_80097500(void);
void func_80097530();
int func_80097568(void);

void func_800973C4(s32 value) {
    D_800ABC50 = value;
}

s32 func_800973D0(void) {
    return D_800ABC50;
}

void func_800973E0(a0, a1, a2, a3) int a0, a1, a2, a3; {
    func_800977C4();
    func_80096F04();
    func_8009774C();
    func_80096F14();
    func_80096F84(0);
    func_80097568();
    func_800976F4(a0, a1, a2, a3);
    D_800ABC50 = 1;
    /* ASM_BARRIER: keep the final global store before the epilogue loads */
    __asm__ volatile("" : : : "memory");
}

void func_80097470(a0, a1, a2, a3) int a0; int a1; int a2; int a3; {
    func_800977C4();
    func_80096F04();
    func_8009774C();
    func_80096F14();
    func_80096F84(0);
    func_80097568();
    func_800976C4(a0, a1, a2, a3);
    D_800ABC50 = 1;
    /* ASM_BARRIER: keeps epilogue stack loads (lw ra/s*) after the final global stores; sched2 otherwise hoists/sinks them */
    __asm__ volatile ("" : : : "memory");
}

s32 func_80097500(void) {
    func_800976D4();
    func_80096F84(0);
    func_80097724();
    return 1;
}

void func_80097530() { func_80097738(); func_800976E4(); func_800975E0(); D_800ABC50 = 0; }

int func_80097568(void) {
    int *p;
    char *s;
    func_80096F04();
    p = D_801143EC;
    s = (char *)p - 4;
    p[0] = (int)func_80097618;
    p[1] = (int)func_80097680;
    D_801143E8 = 0;
    D_801143F4 = 0;
    func_80097714(1, s);
    func_80097704(1, s);
    func_80096F14();
    /* ASM_BARRIER: keeps li v0,1 as a live constant so it is emitted before the epilogue loads (original order) */
    { int r; __asm__("" : "=r"(r) : "0"(1)); return r; }
}
