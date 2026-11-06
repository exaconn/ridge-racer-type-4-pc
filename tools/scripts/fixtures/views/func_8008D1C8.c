/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

#define R(o) (*(unsigned short *)(D_800A6760 + (o)))

/* legacy typing of shared globals in this part of the unit */
#define D_800A6488 (*(void (*(*))(u8, u8 *))&D_800A6488)

#define D_800A648C (*(void (*(*))(u8, u8 *))&D_800A648C)

#undef D_800A6488

#undef D_800A648C

/* legacy typing of shared globals in this part of the unit */
#define D_800A674C ((unsigned char *)D_800A674C)

#define D_800A6750 ((unsigned char *)D_800A6750)

#define D_800A6754 ((unsigned char *)D_800A6754)

#undef D_800A674C

#undef D_800A6750

#undef D_800A6754

/* legacy typing of shared globals in this part of the unit */
#define D_800A674C ((signed char *)D_800A674C)

#define D_800A6754 ((signed char *)D_800A6754)

#undef D_800A674C

#undef D_800A6754

/* legacy typing of shared globals in this part of the unit */
#define D_800A674C ((unsigned char *)D_800A674C)

#define D_800A6750 ((unsigned char *)D_800A6750)

#define D_800A6754 ((unsigned char *)D_800A6754)

#undef D_800A674C

#undef D_800A6750

#undef D_800A6754

extern s32 func_8001E84C();

extern s32 VSync(s32);

extern s32 func_8008B4B8(void);

extern s32 CD_sync(s32, u8 *);

extern const char D_80011AB0[];

extern const char D_80011AB8[];

extern const char D_80011AC8[];

extern s32 D_800A6490;

extern u8 D_800A64A0[4];

extern u8 D_800A64A4;

extern u8 D_800A64A5;

extern char *D_800A64AC[];

extern char *D_800A652C[];

extern s32 D_800A65CC[];

extern s32 D_800A66CC[];

extern volatile u8 *D_800A674C;

extern volatile u8 *D_800A6750;

extern volatile u8 *D_800A6754;

extern u8 D_800A6764[];

extern u8 D_80110318[];

extern s32 D_80110330;

extern s32 D_80110334;

extern const char *D_80110338;

static inline int check_alarm(char **ac, char ** sc, volatile u8 *st) {
    if (VSync(-1) > D_80110330 || D_80110334++ > 0x3C0000) {
        func_800963DC(D_80011A14);
        { u8 s0_ = st[0]; u8 s1_ = st[1]; char *x5 = sc[s1_];
        /* ASM_BARRIER: Load D_80110338 directly into $a1. */
        register const char *a1 __asm__("$5") = D_80110338;
        func_8001E84C(
            D_80011A24,
            a1,
            ac[D_800A64A5],
            sc[s0_],
            x5
        ); }
        CD_flush();
        return -1;
    }
    return 0;
}

static inline void cd_poll_cb(volatile u8 *st, volatile u8 *st1) {
    u8 saved = *D_800A674C & 3;
    s32 res;
    while ((res = func_8008C724()) != 0) {
        if ((res & 4) && D_800A648C != 0) {
            D_800A648C(*st1, D_80110320);
        }
        if ((res & 2) && D_800A6488 != 0) {
            D_800A6488(*st, D_80110318);
        }
    }
    *D_800A674C = saved;
}

s32 CD_cw(u8 arg0, u8 *arg1, u8 *arg2, s32 arg3) {
    s32 i;
    u8 *src;
    u8 *dst;
    u8 *p;

    if (D_800A6490 >= 2) {
        func_8001E84C(D_80011AB0, D_800A64AC[arg0]);
    }
    if (D_800A66CC[arg0] != 0 && arg1 == 0) {
        if (D_800A6490 > 0) {
            func_8001E84C(D_80011AB8, D_800A64AC[arg0]);
        }
        return -2;
    }
    p = arg1;
    {
        /* ASM_BARRIER: Materialize first zero argument in $a0 before the asm barrier. */
        register s32 a0_zero __asm__("$4") = 0;
        /* ASM_BARRIER: Invalidate reload_cse_regs tracking on $a0 so $a1=0 stays move $a1,$zero. */
        __asm__("" : "+r"(a0_zero));
        CD_sync(a0_zero, 0);
    }
    if (arg0 == 2) {
        for (i = 0; i < 4; i++) {
            D_800A64A0[i] = p[i];
        }
    }
    if (arg0 == 0xE) {
        D_800A64A4 = *p;
    }
    D_800A6764[0] = 0;
    if (((s32 (*)[64])D_800A65CC)[0][arg0] != 0) {
        D_800A6764[1] = 0;
    }
    *D_800A674C = 0;
    for (i = 0; i < ((s32 (*)[64])D_800A65CC)[1][arg0]; i++) {
        *D_800A6754 = p[i];
    }
    D_800A64A5 = arg0;
    *D_800A6750 = arg0;
    if (arg3 != 0) {
        return 0;
    }
    D_80110330 = VSync(-1) + 0x3C0;
    D_80110334 = 0;
    D_80110338 = D_80011AC8;
    if (D_800A6764[0] == 0) {
        char **ac = D_800A64AC;
        char **sc = D_800A652C;
        volatile u8 *st = D_800A6764;
        volatile u8 *st1 = st + 1;
        do {
            if (check_alarm(ac, sc, st)) {
                return -1;
            }
            if (func_8008B4B8() != 0) {
                cd_poll_cb(st, st1);
            }
        } while (*st == 0);
    }
    dst = arg2;
    src = D_80110318;
    if (dst != 0) {
        s32 n;
        for (n = 7; n != -1; n--) {
            *dst++ = *src++;
        }
    }
    return -(((volatile u8 *)D_800A6764)[0] == 5);
}
