#include "game/math/shuffle.h"
#include "rr/frame_loop.h"
extern struct { int p; int a[8]; } D_8010E240;
extern int D_8010E264;
extern int func_80096464();
extern int func_80096474();
void func_8005D868()
{
    int i, a, b, x, y, t;
    func_80096474(D_800ABEAC);
    for (i = 7; i >= 0; i--)
        D_8010E240.a[i] = i;
    i = 0;
    do {
        a = func_80096464();
        x = a % 8;
        b = func_80096464();
        y = (x + b % 4) % 7;
        t = D_8010E240.a[x];
        i++;
        D_8010E240.a[x] = D_8010E240.a[y];
        D_8010E240.a[y] = t;
    } while (i < 0x20);
    D_8010E264 = 0;
}
