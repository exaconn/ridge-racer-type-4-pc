#include "rr/bios_devices.h"

extern char D_800113D8[];
extern char D_8010ECD0[];
extern int func_80096484();
extern char *func_80096F74();
int func_8007834C(int a, int b) {
    char sp[32];
    char *s0;
    int n = 0;
    func_80096484(sp, D_800113D8, a, b);
    s0 = D_8010ECD0;
    if (func_80097114(sp, s0) == s0) {
        do { n++; s0 += 0x28; } while (func_80096F74(s0) == s0);
        n = n;
    }
    return n;
}
