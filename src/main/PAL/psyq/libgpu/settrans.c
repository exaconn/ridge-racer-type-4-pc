#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/settrans.h"

void SetSemiTrans(void *arg0, s32 arg1) {
    PolyG4Packet *p = arg0;
    u8 var_v0;

    if (arg1 != 0) {
        var_v0 = p->code | 2;
    } else {
        var_v0 = p->code & 0xFD;
    }
    p->code = var_v0;
}
