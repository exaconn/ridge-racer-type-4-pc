extern int func_80047610();
extern int D_800ABEA4;
extern int D_800ABECC;

void func_80043DE4(a0, a1, a2) char *a0; int *a1; int *a2; {
    int d;
    int q2;
    int r;
    int t;
    int j;
    d = (*(int *)(a0 + 0xF0) << 10) + *(short *)(a0 + 0x170) - a2[2];
    t = d;
    if (d < 0) t = d + 0x3FF;
    r = t >> 10;
    q2 = r;
    r = d - (q2 << 10);
    d = q2;
    if (d < 0) d += D_800ABECC;
    j = d + 1;
    if (D_800ABECC < j) j -= D_800ABECC;
    func_80047610(D_800ABEA4 + d * 60, D_800ABEA4 + j * 60, a2[0], r, a1);
    a1[0] = a1[0] << 2;
    a1[2] = a1[2] << 2;
    a1[1] = a1[1] + (a2[1] << 2);
}
