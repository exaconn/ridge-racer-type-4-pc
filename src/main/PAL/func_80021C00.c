struct S { char pad[0x2AA]; short f; char pad2[0x320 - 0x2AC]; };
extern struct S D_800ABEF8[];
extern char *D_800ACC14;
extern int D_800F3A30;
void func_80021C00(int a0) {
    int a1, a2;
    char *p;
    a2 = a0 - 1;
    if (a0 == 0) {
        a1 = 7;
        a2 = 0;
    } else {
        a1 = a2 * 6 + 1;
    }
    p = D_800ACC14 + (a1 * 0x14 + 0x22688);
    if (D_800ABEF8[a2].f < D_800F3A30) {
        *(short *)(p + 0x10) = 0;
        return;
    }
    *(short *)(p + 0x10) = 0x14;
}
