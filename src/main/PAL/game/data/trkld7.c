/* CC1_FLAGS: -fno-strength-reduce */
extern int func_8001223C(), func_8001292C(), func_80012A58(), func_80012A68(), func_800436E8();
extern char *func_8004D4BC();
extern int func_8004ED38(), func_8004F600(), func_80070108();
extern int D_800A021C;
extern char D_800A0234[];
extern char *D_800ACBB4;
extern char *D_800ACC18;
extern char D_800F4DB8[];
extern char *D_8010C1F0;
extern char D_8010C210[];

int func_8004E70C(void)
{
    int i, i4, r, ret;
    char *s1, *s3;
    char *t;
    switch (D_800A021C) {
    case 0:
        ret = r = func_8001292C(func_8001223C(2, 1), D_8010C1F0);
        if (r == 0) break;
        D_800A021C = D_800A021C + 1;
    case 1:
        if (func_80012A58() == 0) { ret = 0; break; }
        t = D_8010C1F0 + *(int *)(D_8010C1F0 + 4);
        func_80070108(t, 7);
        i = 0;
        func_80012A68(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
        s3 = D_800F4DB8;
        s1 = D_800A0234;
        D_800ACBB4 = D_8010C1F0 + *(int *)(D_8010C1F0 + 8);
        do {
            *(char **)D_8010C210 = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0x14);
            *(char **)(D_8010C210 + 4) = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0x1C);
            func_8004ED38(D_8010C210, *(int *)(s1 + 4), *(int *)(s1 + 8));
            s1 += 0xC;
            *(char **)(s3 + 0x2858) = D_8010C1F0 + *(int *)(D_8010C1F0 + i * 4 + 0xC);
            s3 += 4;
            i++;
        } while (i < 2);
        func_800436E8(D_8010C1F0 + *(int *)(D_8010C1F0 + 0x24));
        t = func_8004D4BC(D_8010C1F0 + *(int *)(D_8010C1F0 + 0x1C));
        D_800ACC18 = t;
        D_8010C1F0 = t;
        D_800A021C = 0;
        func_8004F600();
        ret = 1;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}
