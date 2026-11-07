/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/spu_tick_control.h"
#include "rr/timing.h"
#include "psyq/libapi/vsyncini.h"

extern s32 *D_800A6394;

void trapIntrVSync();
void setIntrVSync(s32 slot,RrSpuTickCallback callback);
void VSync_memclr();

/* legacy typing of shared globals in this part of the unit */
#define InterruptCallback ((int (*)(int, int *))InterruptCallback)
#define D_800A6370 (*(int (*))&D_800A6370)

int *startIntrVSync(void) {
    *D_800A6394 = 0x100;
    Vcount = 0;
    VSync_memclr(&D_800A6370, 8);
    InterruptCallback(0, &trapIntrVSync);
    return &setIntrVSync;
}

#undef InterruptCallback
#undef D_800A6370

/* legacy typing of shared globals in this part of the unit */
#define D_800A6370 (*(void (*(*)[8])())&D_800A6370)

void trapIntrVSync() {
    int i = 0;
    void (**p)() = D_800A6370;
    Vcount++;
    do {
        if (*p) (*p)();
        i++;
        p++;
    } while (i < 8);
}

#undef D_800A6370

void setIntrVSync(s32 slot,RrSpuTickCallback callback)
{
    RrSpuTickCallback *base=D_800A6370;
    RrSpuTickCallback *p=base+slot;
    if(callback!=*p) *p=callback;
}

void VSync_memclr(p, n) int *p; int n; {
    while (n--) *p++ = 0;
}
