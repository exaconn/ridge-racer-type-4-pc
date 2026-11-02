typedef struct { int a, b, c, d; int e[11]; } S3C;
typedef struct { short x, y; int z; } P8;
extern S3C *D_800ABEA4;
extern int D_800ABECC;
extern P8 *D_800ACB9C;
extern int func_800924D4();

short func_8004C454(a0, a1)
int a0; int a1;
{
    int sp[6];
    int i;
    P8 *p0, *p1;
    int o0, o1;
    S3C *s0, *s1;
    i = (a0 - 1) % D_800ABECC;
    if (i < 0) i += D_800ABECC;
    switch (a1) {
    case 0:
        p0 = D_800ACB9C + a0;
        p1 = D_800ACB9C + i;
        o0 = i * 60;
        o1 = a0 * 60;
        s0 = (S3C *)(o0 + (int)D_800ABEA4);
        s1 = (S3C *)(o1 + (int)D_800ABEA4);
        sp[2] = s0->c + p1->x - (s1->c + p0->x);
        sp[3] = s0->d + p1->y - (s1->d + p0->y);
        break;
    case 10:
        sp[2] = D_800ABEA4[i].c - D_800ABEA4[a0].c;
        sp[3] = D_800ABEA4[i].d - D_800ABEA4[a0].d;
        break;
    }
    return func_800924D4(sp[3], sp[2]);
}
