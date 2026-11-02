#include "types.h"

u8 func_80043104(u8 *p, int n) {
    int i;
    u8 s = 0;
    for (i = 0; i < n; i++) {
        s += *p++;
    }
    return s;
}
