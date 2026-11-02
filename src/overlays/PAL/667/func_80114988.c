#include "rr/overlay667_events.h"

s32 ovl667_func_80114988(void) {
    ovl667_D_80118ED8 = func_80096E74(0xF000000B, 0x400, 0x2000, 0);
    ovl667_D_80118EDC = func_80096E74(0xF000000B, 0x8000, 0x1000, ovl667_func_80114D10);
    D_800F2AAC = func_80096F24(ovl667_D_8011487C, 2);
    D_800F2AA8 = func_80096F24(ovl667_D_8011487C, 0x8001);
    func_80096EB4(ovl667_D_80118EDC);
    func_80096EB4(ovl667_D_80118ED8);
    return 0;
}
