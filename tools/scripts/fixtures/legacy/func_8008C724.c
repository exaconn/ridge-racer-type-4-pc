/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
/* RODATA 0x80011A88 0x14 */

#include "types.h"

extern s32 func_8001E84C();
extern s32 func_800963DC(const char *);

extern const char D_80011A40[];
extern const char D_80011A4C[];
extern const char D_80011A68[];
extern const char D_80011A7C[];

extern s32 D_800A6490;
extern s32 D_800A6494;
extern s32 D_800A6498;
extern s32 D_800A649C;
extern u8 D_800A64A5;
extern char *D_800A64AC[];
extern s32 D_800A654C[];
extern s32 D_800A664C[];
extern volatile u8 *D_800A674C;
extern volatile u8 *D_800A6750;
extern volatile u8 *D_800A6754;
extern volatile u8 *D_800A6758;
extern volatile u8 D_800A6764[];

extern u8 D_80110318[];
extern u8 D_80110320[];
extern u8 D_80110328[];

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

s32 func_8008C724(void) {
    volatile u8 intr;
    u8 res[8];
    s32 i;
    s32 j;
    s32 err;

    *D_800A674C = 1;
    err = 0;
    if ((intr = *D_800A6758 & 7) == 0) {
        return 0;
    }
    while (intr != (*D_800A6758 & 7)) {
        intr = *D_800A6758 & 7;
    }
    for (i = 0; i < 8; i++) {
        if ((*D_800A674C & 0x20) == 0) {
            break;
        }
        res[i] = *D_800A6750;
    }
    for (j = i; j < 8; j++) {
        res[j] = 0;
    }
    *D_800A674C = 1;
    *D_800A6758 = 7;
    *D_800A6754 = 7;
    {
        s32 t = intr;
        if (t != 3 || D_800A664C[D_800A64A5] != 0) {
            if ((D_800A6494 & 0x10) == 0 && (res[0] & 0x10) != 0) {
                D_800A649C++;
            }
            D_800A6494 = *(volatile u8 *)res;
            err = D_800A6494 & 0x1D;
            D_800A6498 = res[1];
        }
    }
    {
        s32 t = intr;
        if (t == 5) {
            if (D_800A6490 > 0) {
                func_8001E84C(D_80011A40);
            }
            if (D_800A6490 > 0) {
                func_8001E84C(D_80011A4C, D_800A64AC[D_800A64A5], D_800A6494, D_800A6498);
            }
        }
    }
    switch (intr) {
    case 3:
        if (err) {
            D_800A6764[0] = 5;
            COPY8(D_80110318, res);
            return 2;
        }
        if (D_800A654C[D_800A64A5]) {
            D_800A6764[0] = 3;
            COPY8(D_80110318, res);
            return 1;
        }
        D_800A6764[0] = 2;
        COPY8(D_80110318, res);
        return 2;
    case 2:
        D_800A6764[0] = err ? 5 : 2;
        COPY8(D_80110318, res);
        return 2;
    case 1:
        if (err && i == 1) {
            err = 0;
        }
        {
            u8 v = err ? 5 : 1;
            volatile u8 *p = D_800A6764;
            p[1] = v;
        }
        COPY8(D_80110320, res);
        *D_800A674C = 0;
        *D_800A6758 = 0;
        return 4;
    case 4:
        {
            volatile u8 *p = D_800A6764;
            p[1] = p[2] = 4;
        }
        COPY8(D_80110328, res);
        COPY8(D_80110320, res);
        return 4;
    case 5:
        D_800A6764[0] = D_800A6764[1] = 5;
        COPY8(D_80110318, res);
        COPY8(D_80110320, res);
        return 6;
    default:
        func_800963DC(D_80011A68);
        func_8001E84C(D_80011A7C, intr);
        return 0;
    }
}
