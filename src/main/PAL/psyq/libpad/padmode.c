/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libpad/padintr.h"
extern int (*D_800ABC6C)();
extern int PadChannelRecords;
extern int PadComActive;
extern int PadFirstChannel;
extern int D_800ABCBC;
extern int D_80114428;
extern int D_8011442C;
extern int _padIntPad();
extern int D_8011441C[];

int PadEnableCom(int a0);
void _padSetVsyncParam();

int PadEnableCom(int a0) {
    int s1;
    /* ASM_BARRIER: pins p to s2 to match original register allocation */
    register int *p __asm__("$18");
    int *q;
    int t1, t2;
    s1 = (D_800ABCBC * 2) | (PadFirstChannel == 0);
    if (s1 != a0) {
        PadComActive = 0;
        if (a0 & 1) {
            p = &D_80114428;
            t1 = *p;
            PadFirstChannel = 0;
            if (t1 >= 0x96) {
                D_800ABC6C(PadChannelRecords);
            }
            *p = 0;
        } else {
            PadFirstChannel = 1;
        }
        if (a0 & 2) {
            q = &D_8011442C;
            D_800ABCBC = 1;
            if (*q >= 0x96) {
                D_800ABC6C(PadChannelRecords + 0xF0);
            }
            *q = 0;
        } else {
            D_800ABCBC = 0;
        }
        PadComActive = 1;
    }
    return s1;
}

void _padSetVsyncParam() {
    int *p = D_8011441C;
    p[0] = (int)_padIntPad;
    p[1] = (int)_padIsVsync;
    p[-1] = 0;
    p[2] = 0;
}
