#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt6.h"

void SetLineF2(void *arg0) {
    PolyG4Packet *p = arg0;
    p->len = 3;
    p->code = 0x40;
}
