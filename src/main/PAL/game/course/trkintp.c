#include "types.h"
#include "m2c_macros.h"

#define W(p,o) (*(int *)((char *)(p) + (o)))
#define H(p,o) (*(short *)((char *)(p) + (o)))

extern char *D_800ABEA4;
extern int D_800ABECC;
extern int func_80025C94();
extern int func_80025D0C();

void func_80025D98();
void func_80025EB0(int a, int b);
int func_80025F84(int a0, int a1);
s32 func_80025FBC(s32 arg0, s32 arg1);

void func_80025D98(a, r, c) int a; int *r; int c; {
    int n = (a + 1) % D_800ABECC;
    int c2 = c;
    int t = 0x400 - c2;
    char *p = D_800ABEA4 + a * 0x3C;
    char *q = D_800ABEA4 + n * 0x3C;
    r[0] = (t * W(p,8) + c2 * W(q,8)) / 1024;
    r[1] = (t * H(p,0x10) + c2 * H(q,0x10)) / 1024;
    r[2] = (t * W(p,0xC) + c2 * W(q,0xC)) / 1024;
}

void func_80025EB0(int a, int b)
{
    int s2;
    int s0;
    int x;
    s2 = func_80025D0C(a, b);
    x = a - 2;
    if (x < 0) {
        int d = D_800ABECC - 2;
        x = d + a;
    }
    s0 = func_80025C94(s2, func_80025D0C(x, b), 0x200);
    func_80025C94(s0, func_80025C94(s2, func_80025D0C((a + 2) % D_800ABECC, b), 0x200), 0x200);
}

int func_80025F84(int a0, int a1) {
    int d;
    a0 &= 0xFFF;
    a1 &= 0xFFF;
    if (a0 < a1) d = a1 - a0; else d = a0 - a1;
    if (d > 0x800) d = 0x1000 - d;
    return d;
}

s32 func_80025FBC(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;

    temp_a0 = arg0 & 0xFFF;
    temp_v0 = arg1 & 0xFFF;
    var_v1 = temp_a0 < temp_v0;
    if (var_v1 != 0) {
        var_a0 = temp_v0 - temp_a0;
    } else {
        var_a0 = temp_a0 - temp_v0;
    }
    if (var_a0 >= 0x801) {
        var_a0 = 0x1000 - var_a0;
        var_v1 = var_v1 == 0;
    }
    var_v0 = var_a0;
    if (var_v1 == 0) {
        var_v0 = -var_v0;
    }
    return var_v0;
}
