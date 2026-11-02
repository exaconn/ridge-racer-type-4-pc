#include "types.h"
extern struct { char pad[0x20]; struct { char p[0xC]; s32 f; char q[0x10]; } e[24]; } D_800F2AB8;
int func_80053E88(void) {
    int i;
    for (i = 0; i < 24; i++) {
        if (D_800F2AB8.e[i].f == 0) return i;
    }
    return -1;
}
