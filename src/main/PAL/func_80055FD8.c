#include "types.h"
struct E { s16 a, h; };
int func_80055FD8(struct E *p, int a1) {
    int t = a1 / 10;
    int i = 0;
    while (p[i].h >= 0) {
        if (t < p[i].h) break;
        i++;
    }
    return i;
}
