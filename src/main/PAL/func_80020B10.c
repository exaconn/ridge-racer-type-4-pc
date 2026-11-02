extern int *func_8001FCC8();
extern int func_80093B14();
extern int D_800ACC14;
extern int D_800F4BF0;
extern short D_80107178;

void func_80020B10(a0) int a0;
{
    short sp[4];
    int *s0;
    int a1, d, t;
    int *v;
    int *s1;
    if (a0 == 1) goto L68;
    if (a0 < 2) goto L58;
    if (a0 < 5) goto L7C;
L58:
    a1 = 4;
    s0 = (int *)(D_800ACC14 + 0x70);
    goto LC0;
L68:
    a1 = 8;
    s0 = (int *)(D_800ACC14 + 0x70);
    goto LC0;
L7C:
    if (D_80107178 != 2 || a0 != 3) {
        if (a0 != 4) goto Lb4;
    }
    a1 = 6;
    s0 = (int *)(D_800ACC14 + 0x70);
    goto LC0;
Lb4:
    a1 = 0xC;
    s0 = (int *)(D_800ACC14 + 0xB70);
LC0:
    d = D_800ACC14;
    v = (int *)(a1 * 0x14 + d + 0x22674);
    *v = (*v & 0xFF000000) | (*s0 & 0xFFFFFF);
    *s0 = (*s0 & 0xFF000000) | ((d + 0x22688) & 0xFFFFFF);
    s1 = func_8001FCC8(s0, *(int **)0x1F800000, 5);
    *(int **)0x1F800000 = s1;
    if (a0 >= 2) {
        sp[2] = 0x140;
        sp[0] = 0;
        sp[3] = 0x100;
        t = D_800F4BF0; sp[1] = t << 8;
        func_80093B14(s1, sp);
        *s1 = (*s1 & 0xFF000000) | (*s0 & 0xFFFFFF);
        *s0 = (*s0 & 0xFF000000) | ((int)s1 & 0xFFFFFF);
        *(int **)0x1F800000 = s1 + 3;
    }
}
