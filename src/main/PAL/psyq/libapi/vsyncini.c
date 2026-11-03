/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/spu_tick_control.h"
#include "rr/timing.h"

extern s32 *D_800A6394;

int *func_8008BA24(void);
void func_8008BA7C();
void func_8008BAE8(s32 slot,RrSpuTickCallback callback);
void func_8008BB14();

/* legacy typing of shared globals in this part of the unit */
#define func_8008B394 ((int (*)(int, int *))func_8008B394)
#define D_800A6370 (*(int (*))&D_800A6370)

int *func_8008BA24(void) {
    *D_800A6394 = 0x100;
    D_800A6390 = 0;
    func_8008BB14(&D_800A6370, 8);
    func_8008B394(0, &func_8008BA7C);
    return &func_8008BAE8;
}

#undef func_8008B394
#undef D_800A6370

/* legacy typing of shared globals in this part of the unit */
#define D_800A6370 (*(void (*(*)[8])())&D_800A6370)

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

#undef D_800A6370

void func_8008BAE8(s32 slot,RrSpuTickCallback callback)
{
    RrSpuTickCallback *base=D_800A6370;
    RrSpuTickCallback *p=base+slot;
    if(callback!=*p) *p=callback;
}

void func_8008BB14(p, n) int *p; int n; {
    while (n--) *p++ = 0;
}
