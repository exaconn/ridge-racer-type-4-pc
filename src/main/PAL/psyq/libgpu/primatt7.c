#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt7.h"

void SetLineG2(void *arg0) {
    PolyG4Packet *p = arg0;
    p->len = 4;
    p->code = 0x50;
}
