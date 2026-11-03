/* GCC_VERSION: 2.7.2 */
#include "rr/channel_advance.h"

int func_8009A2D8();
char *func_8009A310();

int func_8009A2D8(a0) char *a0; {
  int i; int r; char *p; i = 0; r = 0x10; p = D_801144D8; for (; i < 2; i++) { if (a0 == p) return r; r += 0x10; p += 0xF0; } return 0xFF;
}

char *func_8009A310(a0) int a0; {
    char *p = D_801144D8;
    if (a0 & 0xF0) p += 0xF0;
    return p;
}
