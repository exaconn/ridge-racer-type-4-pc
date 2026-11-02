/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"

s32 func_8009D8B0(s32 a0, s32 a1, s32 a2) {
    int r;
    D_80114724 = a1;
    if (((u32)a1 - 1) < 0xF) {
        if (!(D_800ABDE4 & 1)) {
            r = 0;
        } else {
            r = 1;
            D_800ABDF4 = 0;
            D_800ABDF8 = 0;
            D_80114730 = 0;
            D_800ABDE8 = 0;
            D_800ABDEC = a0;
        }
        if (r != 0) {
            D_8011472C.value = a2 != 0;
            D_80114870 = 0xB;
            D_80114734 = func_8009D978;
            D_80114738 = func_8009DB18;
            D_800ABDF0 = 1;
            D_800ABDE4 = 2;
            return 1;
        }
        return 0;
    }
    return 0;
}
