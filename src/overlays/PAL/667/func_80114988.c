#include "rr/overlay667_events.h"

s32 ovl667_func_80114988(void) {
    ovl667_D_80118ED8 = OpenEvent(0xF000000B, 0x400, 0x2000, 0);
    ovl667_D_80118EDC = OpenEvent(0xF000000B, 0x8000, 0x1000, ovl667_func_80114D10);
    D_800F2AAC = FileOpen(ovl667_D_8011487C, 2);
    D_800F2AA8 = FileOpen(ovl667_D_8011487C, 0x8001);
    EnableEvent(ovl667_D_80118EDC);
    EnableEvent(ovl667_D_80118ED8);
    return 0;
}
