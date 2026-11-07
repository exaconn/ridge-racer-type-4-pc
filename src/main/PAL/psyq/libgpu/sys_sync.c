/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libgpu/sys_drain.h"
extern int _qin;
extern int _qout;
extern volatile int *DMA2_CHCR;
extern volatile int *GPU_STATUS;
extern int set_alarm();
extern int get_alarm();
int _sync(a0) int a0; {
    int s;
    int v;
    if (a0 == 0) {
        set_alarm();
        goto L5C;
L44:
        _exeque();
        if (get_alarm() != 0) return -1;
L5C:
        if (_qin != _qout) goto L44;
        goto L90;
L80:
        if (get_alarm() != 0) return -1;
L90:
        if (*DMA2_CHCR & 0x01000000) goto L80;
        if (!(*GPU_STATUS & 0x04000000)) goto L80;
        return 0;
    }
    s = (_qin - _qout) & 0x3F;
    if (s != 0) _exeque();
    if (*DMA2_CHCR & 0x01000000) goto L344;
    if (*GPU_STATUS & 0x04000000) goto retS;
L344:
    if (s != 0) return s;
    return 1;
retS:
    return s;
}
