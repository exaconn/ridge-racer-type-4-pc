#include "types.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primatt5.h"

void SetTile(void *arg0) {
    PolyG4Packet *p = arg0;
    p->len = 3;
    p->code = 0x60;
}
