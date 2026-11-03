/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/library.h"
#include "psyq/libcd/s_4.h"

extern int D_801102F8;
extern int D_801102F4;
extern int D_801102EC;
extern int D_801102DC;
extern int D_801102D0;
extern short D_801102D4;
extern void func_8008F814();
extern s32 func_8008C12C();
extern s32 func_8008C55C();
extern s32 func_80096DE0();
extern s32 func_80096F04();
extern s32 func_80096F14();
extern s32 D_800A64A8;
extern s8 *D_800A6800;
extern s8 *D_800A680C;
extern s32 D_80112754;
extern RrLibPackedWord D_80112750;
extern void (*D_80112760)(void);
extern int D_80110348;
extern int func_8008C6A4(), func_8008C5A0();

void func_8008F504(void);
void func_8008F564(void);
void func_8008F5E4(void);
int func_8008F670(int a);

void func_8008F504(void) {
    int a = D_80110314;
    D_801102FC = 0;
    D_801102F8 = 0;
    D_801102F4 = 0;
    D_801102EC = 0;
    func_8008F814(0, a);
    D_801102DC = 0;
    D_801102D4 = 0;
    D_801102D0 = 0;
}

void func_8008F564(void) {
    func_80096F04();
    if (D_800A64A8 == 1) {
        func_80096DE0(0);
        func_80096DB8(0);
    } else {
        func_8008C55C(0);
        func_8008C12C(0);
    }
    *D_800A6800 = 0;
    {
        s8 *port = D_800A680C;
        /* ASM_BARRIER: preserve the pointer's load delay before the final call. */
        __asm__ volatile("nop" : : "r"(port));
        *port = 0;
    }
    func_80096F14();
}

void func_8008F5E4(void)
{
    s32 value;
    /* ASM_BARRIER: Keep the next queue index in the original register until publication. */
    register s32 next __asm__("$3");
    /* ASM_BARRIER: Retain the original indirect-call register while publishing queue state. */
    register void (*callback)(void) __asm__("$4");
    RrLibQueueRecord32 *p = D_80110310 + D_801102F8;
    p->state = 2;
    D_80112750 = p->word1C;
    value = p->value08;
    next = D_801102F4;
    callback = D_80112760;
    D_80112754 = value;
    {
        /* ASM_BARRIER: Preserve the assembler temporary for publishing the next index. */
        register u8 *base __asm__("$1");
        /* ASM_BARRIER: Read callback and next index before publishing state and materializing its address. */
        __asm__ volatile("" : : "r"(next), "r"(callback) : "$1", "memory");
        /* PAL mapping: base + 0x2F8 is &D_801102F8. */
        base = (u8 *)0x80110000U;
        /* ASM_BARRIER: Expose the index publication in the callback-test branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        *(s32 *)(base + 0x2F8) = next;
    }
    if (callback != 0) callback();
    D_801102EC = 0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_80112750 (*(char (*)[])&D_80112750)

int func_8008F670(int a) {
    int r;
    if (D_80110348 == 0) {
        func_8008C5A0(func_8008C6A4(D_80112750) + 1, a);
        r = D_80112754;
    } else {
        r = -1;
    }
    return r;
}

#undef D_80112750
