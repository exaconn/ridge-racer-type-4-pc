/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/timing.h"
#include "rr/library.h"
#include "rr/spu_tick_control.h"
#include "m2c_macros.h"
#include "rr/interrupts.h"

extern volatile unsigned *D_800A5258;
extern volatile unsigned *D_800A525C;
extern volatile unsigned D_800A5260;
extern volatile unsigned D_800A5264;
extern s32 func_800963DC(const char *, ...);
extern s32 func_80096F84(s32);
extern s32 func_80096F94(s32,s32);
extern u16 D_800A52CA;
extern u16 *D_800A6358;

int func_8008B154();
void func_8008B2CC(s32 a0, s32 a1);
void func_8008B364(void);
RrLibInterruptCallback func_8008B394(s32 channel, RrLibInterruptCallback callback);
RrLibInterruptCallback func_8008B3C4(s32 channel, RrLibInterruptCallback callback);
void func_8008B3F4(RrSpuTickCallback callback);
void func_8008B428();
void func_8008B458();
void func_8008B488();
u16 func_8008B4B8(void);
u16 func_8008B4C8(void);
s32 func_8008B4E0(s32 value);

int func_8008B154(a0) int a0; {
    volatile unsigned t;
    unsigned s0;
    unsigned s1;
    volatile unsigned *r;
    register int v __asm__("$2"); /* ASM_BARRIER: first call argument stays in v0 like the original */
    int w;
    s0 = *D_800A5258;
    do {
        t = *D_800A525C;
    } while (t != *D_800A525C);
    s1 = (t - D_800A5260) & 0xFFFF;
    if (a0 < 0) return D_800A6390;
    if (a0 == 1) return s1;
    if (a0 > 0) {
        v = D_800A5264;
        v = v - 1;
        v = v + a0;
    } else {
        v = D_800A5264;
    }
    w = 0;
    if (a0 > 0) w = a0 - 1;
    func_8008B2CC(v, w);
    s0 = *D_800A5258;
    func_8008B2CC(D_800A6390 + 1, 1);
    if (s0 & 0x400000) {
        r = D_800A5258;
        if ((int)(s0 ^ *r) >= 0) {
            do {
            } while (((s0 ^ *r) & 0x80000000) == 0);
        }
    }
    D_800A5264 = D_800A6390;
    do {
        D_800A5260 = *D_800A525C;
    } while (D_800A5260 != *D_800A525C);
    return s1;
}

void func_8008B2CC(s32 a0, s32 a1) {
    volatile u32 cnt = (u32)a1 << 15;
    u32 m;
    if ((s32)D_800A6390 < a0) {
        /* ASM_BARRIER: keeps -1 in a register hoisted before the goto loop (original has li v1,-1 before the loop label) */
        __asm__("" : "=r"(m) : "0"(-1));
loop:
        cnt=cnt-1;
        if (cnt == m) {
            char *page=(char *)0x80010000U;
            /* ASM_BARRIER: Materialize the message page before its call-delay offset. */
            __asm__("" : "=r"(page) : "0"(page));
            func_800963DC(page+0x17F8);
            func_80096F84(0);
            func_80096F94(3, 0);
            return;
        }
        if ((s32)D_800A6390 < a0) goto loop;
    }
}

void func_8008B364(void) {
    D_800A6350->reset();
}

RrLibInterruptCallback func_8008B394(s32 channel, RrLibInterruptCallback callback) {
    return D_800A6350->install(channel, callback);
}

RrLibInterruptCallback func_8008B3C4(s32 channel, RrLibInterruptCallback callback) {
    return D_800A6350->exchange(channel,callback);
}

void func_8008B3F4(RrSpuTickCallback callback)
{
    ((RrSpuTickInterruptDriver20 *)D_800A6350)->install14(4,callback);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

void func_8008B428() { D_800A6350[5](); }

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

void func_8008B458() { D_800A6350[4](); }

#undef D_800A6350

/* legacy typing of shared globals in this part of the unit */
#define D_800A6350 (*(int (**(*))())&D_800A6350)

void func_8008B488() { D_800A6350[6](); }

#undef D_800A6350

u16 func_8008B4B8(void) {
    return D_800A52CA;
}

u16 func_8008B4C8(void) {
    return *D_800A6358;
}

s32 func_8008B4E0(s32 value) {
    s32 previous=*D_800A6358;
    *D_800A6358=value;
    return previous;
}
