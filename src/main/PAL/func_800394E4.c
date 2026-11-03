extern int func_800512C4();
#include "rr/lookup.h"
extern int func_800904F4();
extern char *D_800ABEA4;

void func_800394E4(char *p, int b) {
    int s3;
    int s2;
    int t;
    int a1;
    int s0;
    a1 = func_80055250(4, *(int *)(p + 0xF0));
    s3 = 0;
    s2 = s3;
    if (a1 >= 0) {
        if (a1 == 0) a1 = 3;
        s0 = *(short *)(p + 0x172);
        if (s0 < 0) {
            s0 += 0x100;
            if (s0 > 0) s0 = 0;
        } else {
            s0 -= 0x100;
            if (s0 < 0) s0 = 0;
        }
        if (s0 != 0) {
            s0 = s0 * *(short *)(p + 0x1D8) / 7665;
            if (s0 >= 0x40) {
                s0 = 0x3F;
            } else if (s0 < -0x3F) {
                s0 = -0x3F;
            }
            t = *(int *)(p + 0x54) - 0x800;
            t += *(short *)(D_800ABEA4 + *(int *)(p + 0xF0) * 0x3C + 0x12);
            p = (char *)(t & 0xFFF);
            if (s0 < 0) {
                if ((a1 & 1) > 0) {
                    s2 = -(s0 + s0 * func_800904F4(p) / 4096);
                    s3 = -(s0 + -s0 * func_800904F4(p) / 4096);
                } else goto l16;
            } else {
l16:
                if (s0 > 0) {
                    if ((a1 & 2) > 0) {
                        s3 = s0 + s0 * func_800904F4(p) / 4096;
                        s2 = s0 + -s0 * func_800904F4(p) / 4096;
                    }
                }
            }
        }
    }
    func_800512C4(b, s2, s3);
}
