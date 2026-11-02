extern int func_80043AB0();
extern int func_80090758();
struct S {
    char p0[0x1C];
    int f1C, f20, f24, f28;
    char p2C[0x0C];
    int f38, f3C, f40, f44;
};
extern struct S D_800AC650;
/* same symbol under a 2nd C name: cse cannot reuse the earlier address, so the else arm re-materialises lui/addiu like the original */
extern struct S D_800AC650b __asm__("D_800AC650");
extern int D_800AC690, D_800AC694;

int func_80043C14(a0, a1)
char *a0;
int *a1;
{
    int d[3];
    int sp20[4];
    int s;
    struct S *q;
    int t;
    d[0] = (D_800AC650.f20 / 4 - a1[0]) / 4;
    d[1] = (D_800AC650.f24 / 4 - a1[1]) / 4;
    d[2] = (D_800AC650.f28 / 4 - a1[2]) / 4;
    s = func_80090758(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) * 2;
    if (D_800AC650.f1C != 0) {
        t = s >> 2;
        D_800AC650.f3C = t;
        D_800AC650.f38 = s - t;
        D_800AC650.f44 = 0;
        D_800AC650.f40 = s;
        if (s > 0xFFFF) {
            if (*(short *)(a0 + 0x1EA) != 0) goto L;
        } else {
L:
            D_800AC650.f44 = 1;
        }
        if (*(short *)((char *)a1 + 0x32) - *(int *)(a0 + 0xF0) >= 0) D_800AC694 = 1;
    }
    t = s - D_800AC650.f3C;
    if (D_800AC650.f44 == 0) {
        if (t < 0) {
            D_800AC650.f44 = 1;
        } else {
            q = &D_800AC650b;
            if (q->f40 - s < 0) q->f44 = 1;
        }
    }
    func_80043AB0(a0, sp20, a1[5]);
    return D_800AC690 = s;
}
