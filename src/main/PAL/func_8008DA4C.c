/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */

#include "types.h"

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
extern volatile u32 *D_800A6790;

extern s32 D_80110330;
extern s32 D_80110334;
extern const char *D_80110338[];

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
