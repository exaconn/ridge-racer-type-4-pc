#include "types.h"
#include "rr/badge_rows.h"
#include "rr/draw_environment.h"
#include "m2c_macros.h"
#include "psyq/libgpu/primatt5.h"

struct P12 { int a, b, c; };

typedef struct { int a, b, c; } T;

extern int func_80093B14();
extern int func_80095A44();
extern char D_800ADAF0[];
extern int D_800F4BF0;

void *func_8001FC28(u32 *a0, void *cursor, int a2, int a3, int a4, int a5);
T *func_8001FCC8(u32 *a0, T *a1, unsigned short a2);
void *func_8001FD20(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void *func_8001FDD4(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

void *func_8001FC28(u32 *a0, void *cursor, int a2, int a3, int a4, int a5) {
    struct P12 *a1=cursor;
    short sp[4];
    char *p = D_800ADAF0;
    int t;
    sp[0] = a2;
    if (*(short *)(p + 0x5E) == *(short *)(p + 0x227D6)) {
        sp[1] = a3;
    } else {
        t = D_800F4BF0;
        sp[1] = a3 + (t << 8);
    }
    sp[2] = a4;
    sp[3] = a5;
    func_80093B14(a1, sp);
    func_80095A44(a0, a1++);
    return a1;
}

/* legacy typing of shared globals in this part of the unit */
#define func_80095A44 ((void (*)(u32 *,u32 *))func_80095A44)

T *func_8001FCC8(u32 *a0, T *a1, unsigned short a2) {
    func_80093C24(a1, 0, 1, a2, 0);
    func_80095A44(a0, (u32 *)a1++);
    return a1;
}

#undef func_80095A44

void *func_8001FD20(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    func_80095BB4(arg1);
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, s16 *, 0xC) = (s16) arg4;
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s8 *, 4) = (s8) arg6;
    M2C_FIELD(arg1, s8 *, 5) = (s8) arg7;
    M2C_FIELD(arg1, s8 *, 6) = (s8) arg8;
    func_80095A44(arg0, arg1);
    return arg1 + 0x10;
}

void *func_8001FDD4(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    int i = 0;
    int w = a4 / 2;
    int h = a5 / 2;
    do {
        int x = a2 + w * (i & 1);
        int y = a3;
        if (i >= 2)
            y += h;
        a1 = func_8001FD20(a0, a1, x, y, w, h, a6, a7, a8);
        i++;
    } while (i < 4);
    return a1;
}
