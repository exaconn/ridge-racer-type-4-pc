/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

extern s32 func_8001E84C();
extern s32 func_8008B154(s32);
extern s32 func_8008B4B8(void);
extern s32 func_8008C724(void);
extern void func_8008D65C(void);
extern s32 func_800963DC(const char *);

extern const char D_80011A14[];
extern const char D_80011A24[];
extern const char D_80011AA4[];

extern void (*D_800A6488)(u8, u8 *);
extern void (*D_800A648C)(u8, u8 *);
extern u8 D_800A64A5;
extern char *D_800A64AC[];
extern char *D_800A652C[];
extern volatile u8 *D_800A674C;
extern volatile u8 D_800A6764[];

extern u8 D_80110318[];
extern u8 D_80110320[];
extern u8 D_80110328[];
extern s32 D_80110330;
extern s32 D_80110334;
extern const char *D_80110338[];

static inline int check_alarm(char **ac, char **sc, volatile u8 *st) {
    if (func_8008B154(-1) > D_80110330 || D_80110334++ > 0x3C0000) {
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

s32 func_8008CF00(s32 mode, u8 *result) {
    s32 ret;
    u8 *src;
    u8 *dst;
    s32 i;
    char **ac;
    char **sc;
    volatile u8 *st;
    volatile u8 *st1;
    volatile u8 *st2;

    D_80110330 = func_8008B154(-1) + 0x3C0;
    D_80110334 = 0;
    D_80110338[0] = D_80011AA4;
    ac = D_800A64AC;
    sc = D_800A652C;
    st = D_800A6764;
    st1 = st + 1;
    st2 = st + 2;
    do {
        if (check_alarm(ac, sc, st) != 0) {
            return -1;
        }
        if (func_8008B4B8() != 0) {
            cd_poll_cb(st, st1);
        }
        ret = *st2;
        if (ret != 0) {
            st[2] = 0;
            src = D_80110328;
            if (result != 0) {
                dst = result;
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return ret;
        }
        ret = st2[-1];
        if (ret != 0) {
            st[1] = 0;
            dst = result;
            src = D_80110320;
            if (dst != 0) {
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return ret;
        }
    } while (mode == 0);
    return 0;
}
