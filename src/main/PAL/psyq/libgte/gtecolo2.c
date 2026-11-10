#include "rr_asm.h"

void SetGeomScreen(int a0) {
    RR_GTE_CTC2(a0, 26);
}
