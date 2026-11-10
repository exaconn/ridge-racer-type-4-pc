/* GCC_VERSION: 2.7.2 */
#include "rr/spu_common.h"

int SsSetSerialVol(signed char a0, int a1, int a2) {
    SpuCommonAttributes28 s;
    short x = a1, y = a2;
    if (a0 == 0) {
        s.mask = 0xC0;
        if (x >= 0x80) x = 0x7F;
        if (y >= 0x80) y = 0x7F;
        s.cd.volume.left = x * 0x102;
        s.cd.volume.right = y * 0x102;
    }
    if (a0 == 1) {
        s.mask = 0xC00;
        if (x >= 0x80) x = 0x7F;
        if (y >= 0x80) y = 0x7F;
        s.external.volume.left = x * 0x102;
        s.external.volume.right = y * 0x102;
    }
    ((void (*)(SpuCommonAttributes28 *, int))SpuSetCommonAttr)(&s, a1);
}
