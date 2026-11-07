/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libgpu/sys_drain.h"
extern int D_800ABBE8;
extern int D_800ABBEC;
extern volatile int *D_800ABBD4;
extern volatile int *D_800ABBC8;
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
        if (D_800ABBE8 != D_800ABBEC) goto L44;
        goto L90;
L80:
        if (get_alarm() != 0) return -1;
L90:
        if (*D_800ABBD4 & 0x01000000) goto L80;
        if (!(*D_800ABBC8 & 0x04000000)) goto L80;
        return 0;
    }
    s = (D_800ABBE8 - D_800ABBEC) & 0x3F;
    if (s != 0) _exeque();
    if (*D_800ABBD4 & 0x01000000) goto L344;
    if (*D_800ABBC8 & 0x04000000) goto retS;
L344:
    if (s != 0) return s;
    return 1;
retS:
    return s;
}
