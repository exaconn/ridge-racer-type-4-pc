#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primattr.h"

void SetShadeTex(void *arg0, s32 arg1) {
    PolyG4Packet *p = arg0;
    u8 var_v0;

    if (arg1 != 0) {
        var_v0 = p->code | 1;
    } else {
        var_v0 = p->code & 0xFE;
    }
    p->code = var_v0;
}
