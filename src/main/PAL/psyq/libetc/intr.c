/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/timing.h"
#include "rr/library.h"
#include "rr/spu_tick_control.h"
#include "rr/interrupts.h"
#include "rr/overlay668_stream.h"
#include "rr/interrupt_dispatch.h"

extern volatile unsigned *GPUSTAT, *RCNT1_VALUE, VSyncRcnt1Last, D_800A5264;
extern s32 PsyqPuts(const char *, ...);
extern s32 ChangeClearPAD(s32);
extern s32 ChangeClearRCnt(s32,s32);
extern u16 D_800A52CA;

void v_wait(s32 a0, s32 a1);
LibInterruptCallback InterruptCallback(s32 channel, LibInterruptCallback callback);
LibInterruptCallback DMACallback(s32 channel, LibInterruptCallback callback);
void VSyncCallbacks();
void StopCallback();
void RestartCallback();
u16 CheckCallback(void);
u16 GetIntrMask(void);

int VSync(a0) int a0; {
    volatile unsigned t;
    unsigned s0;
    unsigned s1;
    volatile unsigned *r;
    int v;
    s0 = *GPUSTAT;
    do {
        t = *RCNT1_VALUE;
    } while (t != *RCNT1_VALUE);
    s1 = (t - VSyncRcnt1Last) & 0xFFFF;
    if (a0 < 0) return Vcount;
    if (a0 == 1) return s1;
    v_wait((a0 > 0) ? (v = D_800A5264 - 1, v + a0) : D_800A5264, (a0 > 0) ? (a0 - 1) : 0);
    s0 = *GPUSTAT;
    v_wait(Vcount + 1, 1);
    if (s0 & 0x400000) {
        r = GPUSTAT;
        if ((int)(s0 ^ *r) >= 0) {
            do {
            } while (((s0 ^ *r) & 0x80000000) == 0);
        }
    }
    D_800A5264 = Vcount;
    do {
        VSyncRcnt1Last = *RCNT1_VALUE;
    } while (VSyncRcnt1Last != *RCNT1_VALUE);
    return s1;
}

void v_wait(s32 a0, s32 a1) {
    volatile u32 cnt = a1 * 0x8000U;
    u32 m;
    if ((s32)Vcount < a0) {
        m = -1;
loop:
        cnt=cnt-1;
        if (cnt == m) {
            char *page=(char *)0x80010000U;
            /* ASM_BARRIER: Materialize the message page before its call-delay offset. */
            __asm__("" : "=r"(page) : "0"(page));
            PsyqPuts(page+0x17F8);
            ChangeClearPAD(0);
            ChangeClearRCnt(3, 0);
            return;
        }
        if ((s32)Vcount < a0) goto loop;
    }
}

void ResetCallback(void) {
    IntrDriverTablePtr->reset();
}

LibInterruptCallback InterruptCallback(s32 channel, LibInterruptCallback callback) {
    return IntrDriverTablePtr->install(channel, callback);
}

LibInterruptCallback DMACallback(s32 channel, LibInterruptCallback callback) {
    return IntrDriverTablePtr->exchange(channel,callback);
}

void VSyncCallback(SpuTickCallback callback)
{
    ((SpuTickInterruptDriver20 *)IntrDriverTablePtr)->install14(4,callback);
}

void VSyncCallbacks() { ((void (**)(void))IntrDriverTablePtr)[5](); }

void StopCallback() { ((void (**)(void))IntrDriverTablePtr)[4](); }

void RestartCallback() { ((void (**)(void))IntrDriverTablePtr)[6](); }

u16 CheckCallback(void) {
    return D_800A52CA;
}

u16 GetIntrMask(void) {
    return *I_MASK;
}

s32 SetIntrMask(s32 value) {
    s32 previous=*I_MASK;
    *I_MASK=value;
    return previous;
}
