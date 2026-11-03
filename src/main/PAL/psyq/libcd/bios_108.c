/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "rr_asm.h"
#include "rr/cd.h"
#include "rr/library.h"

extern s32 func_8001E84C();
extern s32 func_8008B154(s32);
extern void func_8008D65C(void);
extern s32 func_800963DC(const char *);
extern const char D_80011A14[];
extern const char D_80011A24[];
extern const char D_80011B20[];
extern u8 D_800A64A5;
extern char *D_800A64AC[];
extern char *D_800A652C[];
extern volatile u8 D_800A6764[];
extern s32 D_80110330;
extern s32 D_80110334;
extern const char *D_80110338[];
/* $at as the %hi() base: with $1 allocatable cc1 2.8.1 emits lui $1,%hi / sw %lo($1) and its reorg fills the jr/jal delay slot with the sw (see DECOMP_GUIDE "delay-slot store"). */
extern int D_800A6730;
extern s32 func_8008C724(void);

s32 func_8008DA4C(s32 mode);
s32 func_8008DBB4(u8 *a0, u32 a1);
s32 func_8008DCB4(u8 *a, u32 b);
void func_8008DDA0(int a);
void func_8008DDAC(void);

static inline int check_alarm(s32 limit, char **ac, volatile u8 *st, char **sc) {
    if (func_8008B154(-1) > D_80110330 || D_80110334++ > limit) {
        func_800963DC(D_80011A14);
        {
            u8 s0_ = st[0];
            u8 s1_ = st[1];
            char *x5 = sc[s1_];
            func_8001E84C(
                D_80011A24,
                D_80110338[0],
                ac[D_800A64A5],
                sc[s0_],
                x5
            );
        }
        func_8008D65C();
        return -1;
    }
    return 0;
}

s32 func_8008DA4C(s32 mode) {
    s32 limit;
    char **ac;
    volatile u8 *st;
    char **sc;

    s32 t = func_8008B154(-1);
    /* ASM_BARRIER: Set allocno priorities for s2..s4 (mode, ac, limit). */
    __asm__("" : : "r"(mode), "r"(mode));
    D_80110330 = t + 0x3C0;
    D_80110334 = 0;
    D_80110338[0] = D_80011B20;
    limit = 0x3C0000;
    ac = D_800A64AC;
    st = D_800A6764;
    sc = D_800A652C;
    do {
        if (check_alarm(limit, ac, st, sc) != 0) {
            return -1;
        }
        if ((*D_800A6790 & 0x1000000) == 0) {
            return 0;
        }
    } while (mode == 0);
    return 1;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A6764 (*(u8 (*)[2])&D_800A6764)

s32 func_8008DBB4(u8 *a0, u32 a1) {
    volatile unsigned char *p;
    *D_800A674C = 0;
    *D_800A6758 = 0x80;
    *D_800A6780 = 0x20943;
    /* Ordinary write views retain the original setup and teardown scheduling. */
    *(u32 *)D_800A675C = 0x1323;
    *D_800A6784 |= 0x8000;
    *D_800A6788 = a0;
    *D_800A678C = a1 | 0x10000;
    p = D_800A674C;
    /* ASM_BARRIER: orig has the load-delay nop BEFORE the loop label; maspsx emits it inside the loop */
    RR_NOP_DEP();
    do {
    } while (!(*p & 0x40));
    *D_800A6790 = 0x11000000;
    if (*D_800A6790 & 0x01000000) {
        do {
        } while (*D_800A6790 & 0x01000000);
    }
    *(u32 *)D_800A675C = 0x1325;
    return 0;
}

s32 func_8008DCB4(u8 *a, u32 b) {
    volatile int x;
    *D_800A674C = 0;
    *D_800A6758 = 0x80;
    *D_800A6780 = 0x21020843;
    *D_800A675C = 0x1325;
    *D_800A6784 |= 0x8000;
    *D_800A6788 = a;
    *D_800A678C = b | 0x10000;
    while (!(*D_800A674C & 0x40)) {
    }
    *D_800A6790 = 0x11400100;
    x = *D_800A6790;
    return 0;
}

void func_8008DDA0(int a) { D_800A6730 = a; }

void func_8008DDAC(void) {
    u8 *p=&D_800A6764[1];
    u8 *q=p-1;
    s32 saved=*D_800A674C & 3;
    s32 result;
    while((result=func_8008C724())) {
        if(result & 4) {
            if(D_800A648C) {
                /* ASM_BARRIER: Load the byte argument before preparing its data-page argument. */
                register s32 argument=*p;
                /* ASM_BARRIER: Retain the callback's data-page argument for its call delay slot. */
                register u8 *page __asm__("$5");
                /* ASM_BARRIER: Materialize the callback data page only on its active path. */
                __asm__ volatile("" : : "r"(argument) : "$5");
                page=(u8 *)0x80110000U;
                /* ASM_BARRIER: Keep the data-page offset separate for call scheduling. */
                __asm__("" : "+r"(page));
                D_800A648C(argument,page+0x320);
            }
        }
        if(result & 2) {
            if(D_800A6488) {
                /* ASM_BARRIER: Load the byte argument before preparing its data-page argument. */
                register s32 argument=*q;
                /* ASM_BARRIER: Retain the callback's data-page argument for its call delay slot. */
                register u8 *page __asm__("$5");
                /* ASM_BARRIER: Materialize the callback data page only on its active path. */
                __asm__ volatile("" : : "r"(argument) : "$5");
                page=(u8 *)0x80110000U;
                /* ASM_BARRIER: Keep the data-page offset separate for call scheduling. */
                __asm__("" : "+r"(page));
                D_800A6488(argument,page+0x318);
            }
        }
    }
    *D_800A674C=saved;
}

#undef D_800A6764
