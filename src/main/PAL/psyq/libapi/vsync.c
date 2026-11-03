/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu_tick_control.h"

extern int D_800A6390;
extern void (*D_800A6370[8])();

void func_8008BA7C();
void func_8008BAE8(s32 slot,RrSpuTickCallback callback);
void func_8008BB14();

void func_8008BA7C() {
    int i = 0;
    void (**p)() = D_800A6370;
    D_800A6390++;
    do {
        if (*p) (*p)();
        i++;
        p++;
    } while (i < 8);
}

void func_8008BAE8(s32 slot,RrSpuTickCallback callback)
{
    RrSpuTickCallback *base=D_800A6370;
    RrSpuTickCallback *p=base+slot;
    if(callback!=*p) *p=callback;
}

void func_8008BB14(p, n) int *p; int n; {
    while (n--) *p++ = 0;
}
