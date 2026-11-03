#include "game/car/carphys1.h"
#include "rr/menu_overlay_runtime.h"
#include "game/car/carphys2.h"
extern int func_80025FBC();
#define S(p, o) (*(short *)((p) + (o)))
#define U(p, o) (*(unsigned short *)((p) + (o)))
#define W(p, o) (*(int *)((p) + (o)))
void func_800247FC(a0)
char *a0;
{
    char *s1;
    char *s2;
    int v1, a1, d;
    register int v asm("$2");
    int t;
    s2 = a0 + 0x1F4;
    s1 = *(char **)(s2 + 0x1C);
    v1 = W(a0, 0x1E0) * func_800904F4(S(a0, 0x1D4) - W(a0, 0x54));
    if (v1 < 0) v1 += 0xFFF;
    U(a0, 0x1D8) = U(a0, 0x1D8) + (v1 >> 12);
    if (S(s1, 0xA) == 0) {
        if (S(s2, 0x3C) > 0) {
            S(a0, 0x1D4) = U(a0, 0x1D4) + ((func_80025FBC(S(a0, 0x1D4), func_80024BCC(a0)) * S(s2, 0x3C)) >> 2) / 60;
        }
    }
    a1 = func_80025FBC(S(a0, 0x1D4), W(a0, 0x54));
    if (S(s1, 0xA) > 0) {
        if (S(a0, 0x1DE) < 0) {
            if (S(a0, 0x1D8) < 0x3C0) {
                v = S(a0, 0x1DC);
                t = S(s1, 0x1C);
                if (v < 0) v = -v;
                v >>= 4;
            } else {
                v = S(a0, 0x1DC);
                t = S(s1, 0x1C);
                if (v < 0) v = -v;
                v >>= 3;
            }
        } else {
            v = S(a0, 0x1DC);
            t = S(s1, 0x1C);
            if (v < 0) v = -v;
            v >>= 2;
        }
        d = v / (t + 1) + 4;
    } else {
        d = S(s2, 0x3C) + 4;
        t = S(s1, 0x1C);
        if (t > 0) {
            v = S(a0, 0x1DC);
            if (v < 0) v = -v;
            d += v / t;
        }
    }
    if (S(a0, 0x1B8) == 0) {
        if (a1 < d && -d < a1) {
            S(a0, 0x1D4) = U(a0, 0x1D4) + (a1 >> 1);
        } else {
            S(a0, 0x1D4) = U(a0, 0x1D4) + a1 / d;
        }
    }
    U(a0, 0x1D4) = U(a0, 0x1D4) & 0xFFF;
}
