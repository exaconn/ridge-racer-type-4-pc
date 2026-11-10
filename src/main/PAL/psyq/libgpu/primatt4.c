#include "types.h"
#include "rr/gpu.h"

void SetSprt(void *arg0) {
    PolyG4Packet *p = arg0;
    p->len = 4;
    p->code = 0x64;
}
