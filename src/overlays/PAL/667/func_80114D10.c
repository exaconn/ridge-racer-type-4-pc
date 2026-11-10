#include "rr/overlay667_events.h"

s32 ovl667_func_80114D10(void) {
    union {u32 bits;s32 value;} count;
    count.bits=ovl667_D_80118EE0+1U;
    ovl667_D_80118EE0=count.value;
    ovl667_D_80118EE8 = 0;
    return 0;
}
