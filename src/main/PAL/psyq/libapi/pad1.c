/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"
#include "psyq/libapi/pad2.h"
#include "psyq/libapi/pad1.h"
#include "psyq/libapi/pad2.h"
#include "rr/interrupts.h"
#include "rr/spu_tick_control.h"

extern s32 D_800ABC50;
extern int ChangeClearPAD();
extern int PAD_init2();
extern int _patch_pad();
extern int _remove_ChgclrPAD();
extern int InitPAD2();
extern int StartPAD2();
extern int EnablePAD();
extern int StopPAD2();
extern int DisablePAD();
extern int SysEnqIntRP();
extern int SysDeqIntRP();
extern int D_801143F4;
extern int D_801143EC[];

void SetInitPadFlag(s32 value);
s32 ReadInitPadFlag(void);
void PAD_init();
void InitPAD();
s32 StartPAD(void);
void StopPAD();
int func_80097568(void);

void SetInitPadFlag(s32 value) {
    D_800ABC50 = value;
}

s32 ReadInitPadFlag(void) {
    return D_800ABC50;
}

void PAD_init(a0, a1, a2, a3) int a0, a1, a2, a3; {
    _remove_ChgclrPAD();
    EnterCriticalSection();
    _patch_pad();
    ExitCriticalSection();
    ChangeClearPAD(0);
    func_80097568();
    PAD_init2(a0, a1, a2, a3);
    D_800ABC50 = 1;
    /* ASM_BARRIER: keep the final global store before the epilogue loads */
    __asm__ volatile("" : : : "memory");
}

void InitPAD(a0, a1, a2, a3) int a0; int a1; int a2; int a3; {
    _remove_ChgclrPAD();
    EnterCriticalSection();
    _patch_pad();
    ExitCriticalSection();
    ChangeClearPAD(0);
    func_80097568();
    InitPAD2(a0, a1, a2, a3);
    D_800ABC50 = 1;
    /* ASM_BARRIER: keeps epilogue stack loads (lw ra/s*) after the final global stores; sched2 otherwise hoists/sinks them */
    __asm__ volatile ("" : : : "memory");
}

s32 StartPAD(void) {
    StartPAD2();
    ChangeClearPAD(0);
    EnablePAD();
    return 1;
}

void StopPAD() { DisablePAD(); StopPAD2(); RemovePatchPad(); D_800ABC50 = 0; }

int func_80097568(void) {
    int *p;
    char *s;
    EnterCriticalSection();
    p = D_801143EC;
    s = (char *)p - 4;
    p[0] = (int)func_80097618;
    p[1] = (int)func_80097680;
    D_801143E8 = 0;
    D_801143F4 = 0;
    SysDeqIntRP(1, s);
    SysEnqIntRP(1, s);
    ExitCriticalSection();
    /* ASM_BARRIER: keeps li v0,1 as a live constant so it is emitted before the epilogue loads (original order) */
    { int r; __asm__("" : "=r"(r) : "0"(1)); return r; }
}
