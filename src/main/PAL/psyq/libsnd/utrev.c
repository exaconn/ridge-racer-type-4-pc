/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libspu/smutil.h"
struct S { char pad[0x1AA]; unsigned short f; };
extern struct S *D_800A47D8;
extern int D_800A484C, D_800A4850, D_800A4854;
int func_80086694(int a0) {
    switch (a0) {
    case 0:
        D_800A484C = 0;
        D_800A47D8->f &= 0xFF7F;
        break;
    case 1:
        if (D_800A4850 != a0 && func_8007FCD4(D_800A4854)) {
            D_800A484C = 0;
            D_800A47D8->f &= 0xFF7F;
        } else {
            D_800A484C = a0;
            D_800A47D8->f |= 0x80;
        }
        break;
    }
    return D_800A484C;
}
