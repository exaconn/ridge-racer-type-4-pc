#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt3.h"

void SetPolyFT3(void *arg0) {
    PolyG4Packet *p = arg0;
    p->len = 7;
    p->code = 0x24;
}
