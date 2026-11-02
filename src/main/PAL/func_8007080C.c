extern int func_80095AC4();
extern int func_80095BF4();
extern int D_8010E738;
extern int D_8010E740[];
extern int D_8010E768[];
extern int D_8010E78C;
void func_8007080C(a0)
    int a0;
{
    int s0, s1, s2, s4;
    register int s3 asm("$19");
    int *p, *q, *r;
    int b;
    int t3;
    char *m;
    D_8010E738 = a0;
    a0 = a0 + 0xB34C;
    s0 = a0 + 0x4D8;
    s3 = 0;
    D_8010E78C = a0;
    do {
        s4 = 0;
        s2 = 0;
        do {
            t3 = s3 * 4;
            m = (char *)D_8010E78C + s2;
            m = m + t3;
            *(int *)(m + 0xE8) = s0;
            *(int *)(m + 0xF0) = 0;
            func_80095BF4(s0);
            s0 += 0x14;
            s1 = 1;
            do {
                func_80095BF4(s0);
                func_80095AC4(s0 - 0x14, s0);
                s1++;
                s0 += 0x14;
            } while (s1 < 0xFF);
            s4++;
            s2 += 0xF8;
        } while (s4 < 5);
        s3++;
    } while (s3 < 2);
    p = (int *)s0;
    b = s0;
    s3 = 0;
    q = D_8010E768;
    r = D_8010E740;
    do {
        *r = b + *p;
        p++;
        s3++;
        *q = b + *p;
        p++;
        r++;
        q++;
    } while (s3 < 9);
}
