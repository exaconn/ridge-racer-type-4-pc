/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libgpu/sys_drain.h"
#include "rr/gpu.h"
#include "rr/interrupts.h"
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

extern int DMACallback(int, int (*)());
extern struct GpuSys D_800ABAC0;
extern volatile unsigned int *D_800ABBC8;
extern volatile unsigned int *D_800ABBD4;
extern volatile int D_800ABBE8;
extern int D_800ABBF0;
extern volatile struct GpuQueue D_801128D8[];

void _addque(int a, int b, int c);
int _addque2(GpuOpFunc op, int *p0, int len, int p1);

void _addque(int a, int b, int c) { _addque2(a, b, 0, c); }

int _addque2(GpuOpFunc op, int *p0, int len, int p1) {
    int i;
    struct GpuSys *sys;

    set_alarm();
    while (((D_800ABBE8 + 1) & 0x3F) == D_800ABBEC) {
        if (get_alarm() != 0) {
            return -1;
        }
        _exeque();
    }
    D_800ABBF0 = SetIntrMask(0);
    sys = &D_800ABAC0;
    sys->unk8 = 1;
    if (sys->enabled == 0 || (D_800ABBE8 == D_800ABBEC && (*D_800ABBD4 & 0x01000000) == 0 && sys->unkC == 0)) {
        while ((*D_800ABBC8 & 0x04000000) == 0)
;
        op(p0, p1);
        SetIntrMask(D_800ABBF0);
        return 0;
    }
    DMACallback(2, _exeque);
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
    SetIntrMask(D_800ABBF0);
    _exeque();
    return (D_800ABBE8 - D_800ABBEC) & 0x3F;
}
