#include "rr/badge_rows.h"

void *func_8001FDD4(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    int i = 0;
    int w = a4 / 2;
    int h = a5 / 2;
    do {
        int x = a2 + w * (i & 1);
        int y = a3;
        if (i >= 2)
            y += h;
        a1 = func_8001FD20(a0, a1, x, y, w, h, a6, a7, a8);
        i++;
    } while (i < 4);
    return a1;
}
