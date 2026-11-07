/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

#define COPY8(dst_arr, src_arr) \
    { \
        u8 *d = (dst_arr); \
        u8 *s = (src_arr); \
        s32 n; \
        if (d != 0) { \
            for (n = 7; n != -1; n--) { \
                *d++ = *s++; \
            } \
        } \
    }

extern char *CD_comstr[];

extern volatile u8 D_800A6764[];

extern u8 D_80110318[];

extern s32 VSync(s32);

extern s32 CheckCallback(void);

extern const char D_80011A9C[];

extern char *CD_intstr[];

extern s32 D_80110330;

extern s32 D_80110334;

extern const char *D_80110338[];

static inline int check_alarm(char **ac, char **sc, volatile u8 *st) {
    if (VSync(-1) > D_80110330 || D_80110334++ > 0x3C0000) {
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
        CD_flush();
        return -1;
    }
    return 0;
}

static inline void cd_poll_cb(volatile u8 *st, volatile u8 *st1) {
    u8 saved = *D_800A674C & 3;
    s32 res;
    while ((res = getintr()) != 0) {
        if ((res & 4) && CD_cbready != 0) {
            CD_cbready(*st1, D_80110320);
        }
        if ((res & 2) && CD_cbsync != 0) {
            CD_cbsync(*st, D_80110318);
        }
    }
    *D_800A674C = saved;
}

s32 CD_sync(s32 mode, u8 *result) {
    s32 ret;
    u8 *src;
    u8 *dst;
    s32 i;
    char **ac;
    char **sc;
    volatile u8 *st;
    volatile u8 *st1;

    s32 t = VSync(-1);
    D_80110330 = t + 0x3C0;
    D_80110334 = 0;
    D_80110338[0] = D_80011A9C;
    ac = CD_comstr;
    sc = CD_intstr;
    st = D_800A6764;
    st1 = st + 1;
    do {
        if (check_alarm(ac, sc, st) != 0) {
            return -1;
        }
        if (CheckCallback() != 0) {
            cd_poll_cb(st, st1);
        }
        ret = *st;
        if (ret == 2 || ret == 5) {
            *st = 2;
            dst = result;
            src = D_80110318;
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
