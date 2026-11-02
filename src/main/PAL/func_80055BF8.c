#include "types.h"
extern int func_80090EC4();
void func_80055BF8(int a0, int *a1, u16 *a2, int *a3) {
    s16 t[3];
    t[0] = a2[0];
    t[1] = a2[2];
    t[2] = a2[4];
    func_80090EC4(a0, t, a3);
    a3[0] += a1[0];
    a3[1] += a1[1];
    a3[2] += a1[2];
}
