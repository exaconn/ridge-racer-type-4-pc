extern int D_800ABEA4;
extern int D_800ABECC;
extern short D_800AD522;
void func_80043AB0(a0, a1, a2) char *a0; int *a1; int a2;
{
    int v0, a3;
    int t0, t1;
    int v1;
    int a2b;
    if (D_800AD522 == 0) {
        v0 = *(int *)(a0 + 0xF0) - ((a2 - *(short *)(a0 + 0x170)) >> 10);
        a3 = v0 - 1;
        if (v0 < 0) {
            v0 += D_800ABECC;
            a3 = v0 - 1;
        }
        if (a3 < 0) a3 += D_800ABECC;
    } else {
        v0 = *(int *)(a0 + 0xF0) + ((a2 - *(short *)(a0 + 0x170)) >> 10);
        a3 = v0 + 1;
        if (v0 >= D_800ABECC) {
            v0 -= D_800ABECC;
            a3 = v0 + 1;
        }
        if (a3 >= 0) a3 -= D_800ABECC;
    }
    t0 = D_800ABEA4 + v0 * 0x3C;
    t1 = D_800ABEA4 + a3 * 0x3C;
    v1 = (a2 - *(short *)(a0 + 0x170)) & 0x3FF;
    a2 = *(int *)(t0 + 8);
    a1[0] = (((*(int *)(t1 + 8) - a2) * v1) / 1024 + a2) * 4;
    a2 = *(short *)(t0 + 0x10);
    a1[1] = ((*(short *)(t1 + 0x10) - a2) * v1) / 1024 + a2;
    a2 = *(int *)(t0 + 0xC);
    a1[2] = (((*(int *)(t1 + 0xC) - a2) * v1) / 1024 + a2) * 4;
}
