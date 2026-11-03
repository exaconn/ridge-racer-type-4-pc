/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/gpu.h"

struct GpuSys {
    unsigned char pad0;
    unsigned char enabled;
    unsigned char pad2[6];
    int unk8;
    int unkC;
};

struct GpuQueue {
    int (*op)();
    int *p0;
    int p1;
    int buf[21];
};

typedef int (*GpuOpFunc)();

extern void func_80095360(void);
extern int func_80095394(void);
extern int func_80094E74(void);
extern int func_8008B4E0(int);
extern int func_8008B3C4(int, int (*)());
extern struct GpuSys D_800ABAC0;
extern volatile unsigned int *D_800ABBC8;
extern volatile unsigned int *D_800ABBD4;
extern volatile int D_800ABBE8;
extern volatile int D_800ABBEC;
extern int D_800ABBF0;
extern volatile struct GpuQueue D_801128D8[];

void func_80094B28(unsigned long address);
s32 func_80094B70(s32 a);
void func_80094BA0(int a, int b, int c);
int func_80094BC4(GpuOpFunc op, int *p0, int len, int p1);

void func_80094B28(unsigned long address) {
    *D_800ABBC8=0x04000002;
    *D_800ABBCC=(u32)address;
    *D_800ABBD0=0;
    *(u32 *)D_800ABBD4=0x01000401;
}

s32 func_80094B70(s32 a) {
    *D_800ABBC8 = a | 0x10000000;
    return *D_800ABBC4 & 0xFFFFFF;
}

void func_80094BA0(int a, int b, int c) { func_80094BC4(a, b, 0, c); }

int func_80094BC4(GpuOpFunc op, int *p0, int len, int p1) {
    int i;
    struct GpuSys *sys;

    func_80095360();
    while (((D_800ABBE8 + 1) & 0x3F) == D_800ABBEC) {
        if (func_80095394() != 0) {
            return -1;
        }
        func_80094E74();
    }
    D_800ABBF0 = func_8008B4E0(0);
    sys = &D_800ABAC0;
    sys->unk8 = 1;
    if (sys->enabled == 0 || (D_800ABBE8 == D_800ABBEC && (*D_800ABBD4 & 0x01000000) == 0 && sys->unkC == 0)) {
        while ((*D_800ABBC8 & 0x04000000) == 0)
;
        op(p0, p1);
        func_8008B4E0(D_800ABBF0);
        return 0;
    }
    func_8008B3C4(2, func_80094E74);
    if (len != 0) {
        for (i = 0; i < len / 4; i++) {
            D_801128D8[D_800ABBE8].buf[i] = p0[i];
        }
        D_801128D8[D_800ABBE8].p0 = D_801128D8[D_800ABBE8].buf;
    } else {
        D_801128D8[D_800ABBE8].p0 = p0;
    }
    D_801128D8[D_800ABBE8].p1 = p1;
    D_801128D8[D_800ABBE8].op = op;
    D_800ABBE8 = (D_800ABBE8 + 1) & 0x3F;
    func_8008B4E0(D_800ABBF0);
    func_80094E74();
    return (D_800ABBE8 - D_800ABBEC) & 0x3F;
}
