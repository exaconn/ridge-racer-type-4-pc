#include "rr_asm.h"
#include "psyq/libgte/gtemath3.h"
int NormalClip(int a0, int a1, int a2) {
    int v;
    RR_GTE_MTC2(a0, 12);
    RR_GTE_MTC2(a2, 14);
    RR_GTE_MTC2(a1, 13);
    RR_GTE_CMD_2NOP(0x4B400006);
    RR_GTE_MFC2(v, 24);
    return v;
}
