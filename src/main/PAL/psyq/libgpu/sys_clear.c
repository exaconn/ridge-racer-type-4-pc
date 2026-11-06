/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/library.h"
#include "m2c_macros.h"
#include "psyq/libgpu/sys_drain.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/gpu.h"

extern struct { char p[0x18]; int (*f18)(); } *D_800ABAB8;
extern volatile unsigned *D_800ABBD4;
extern volatile unsigned *D_800ABBC8;
extern int D_800ABBFC;
extern int D_800ABC00;

int DrawOTag2();
void GPU_memset(s8 *arg0, s8 arg1, s32 arg2);

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAC2 (*(unsigned char (*))&D_800ABAC2)
#define D_800ABABC (*(int (*(*))())&D_800ABABC)

int DrawOTag2(a0) int a0; {
    if (D_800ABAC2 >= 2) D_800ABABC(D_80011F38, a0);
    D_800ABBFC = VSync(-1) + 0xF0;
    D_800ABC00 = 0;
    while ((*D_800ABBD4 & 0x1000000) || (*D_800ABBC8 & 0x4000000) == 0) {
        if (func_80095394() != 0) return -1;
    }
    func_8008B3C4(2, func_80095990);
    D_800ABAB8->f18(a0);
    return 0;
}

#undef D_800ABAC2
#undef D_800ABABC

void func_80095990(void) {
    func_8008B3C4(2,func_80094E74);
}

void GPU_memset(s8 *arg0, s8 arg1, s32 arg2) {
    s32 var_v0;
    s8 *var_a0;

    var_a0 = arg0;
    var_v0 = arg2 - 1;
    if (arg2 != 0) {
        do {
            *var_a0 = arg1;
            var_v0 -= 1;
            var_a0 += 1;
        } while (var_v0 != -1);
    }
}
