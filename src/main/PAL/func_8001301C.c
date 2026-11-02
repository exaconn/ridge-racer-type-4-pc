extern int func_80096434();
extern int D_800ACC18;
extern int *D_800F3A08[];
struct A { char p[0x14]; unsigned f14; char q[0x20]; unsigned f38; };
struct Q { int a; int v; };
struct E {
    struct A *src; int pad0; int z8, zC, z10;
    struct Q q1[4];
    int z34, z38;
    struct Q q2[4];
    int z5c, z60, z64, z68, z6c, z70;
    int pad74[4];
    struct Q q3[8];
    int fC4;
};
extern struct E D_8010B2E0[];

void func_8001301C(struct A *a0, int a1)
{
    int s1;
    int a3, v1, t, u;
    struct E *s0;
    struct Q *p;

    s0 = (struct E *)((char *)D_8010B2E0 + a1 * 0x320);
    a3 = D_800ACC18;
    u = ((a0->f14 + 2) / 3) * 0x18;
    t = ((a0->f38 + 2) / 3) * 0x18;
    s1 = 4;
    do {
        p = s0->q1;
        v1 = 3;
        s0->src = a0;
        s0->z8 = 0;
        s0->zC = 0;
        s0->z10 = 0;
        s0->z34 = 0;
        s0->z38 = 0;
        s0->z5c = 0;
        s0->z60 = 0;
        s0->z64 = 0;
        s0->z68 = 0;
        s0->z6c = 0;
        s0->z70 = 0;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += u;
        } while (v1 >= 0);
        p = s0->q2;
        v1 = 3;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += t;
        } while (v1 >= 0);
        p = s0->q3;
        v1 = 7;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += t;
        } while (v1 >= 0);
        s0++;
        s1 -= 1;
    } while (s1 > 0);
    D_800ACC18 = a3;
    if (*(unsigned char *)((char *)D_800F3A08[a1] + 0xA) == 2) {
        s0 = (struct E *)((char *)D_8010B2E0 + a1 * 0x320);
        s1 = 4;
        do {
            s1 -= 1;
            s0->fC4 = D_800ACC18;
            func_80096434(D_800ACC18, 0x100);
            s0++;
            D_800ACC18 += 0x100;
        } while (s1 > 0);
    }
}
