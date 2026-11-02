extern int D_800F2EB4;
extern unsigned char D_800F4C38[];
extern unsigned int *D_8010E690;

void func_8006E8F8() {
    int *t1;
    int v;
    int t0;
    unsigned int *p;
    unsigned int n;
    unsigned int i;
    unsigned int t2;
    unsigned char *pb;

    t0 = 0;
    t1 = (int *)D_8010E690;
    t2 = *t1;
    t1 = (int *)((int)t1 + (t2 * 4 + 4));
    if (D_800F4C38[0] != 0) {
        pb = D_800F4C38;
        do {
            i = 0;
            n = *D_8010E690;
            v = D_800F2EB4 + pb[1] * 0x50;
            if (n != 0) {
                p = D_8010E690;
loop:
                if (v != p[1]) {
                    i++;
                    p++;
                    if (i < n) goto loop;
                }
            }
            if (i == *D_8010E690) {
                *t1 = v;
                t1++;
                t2++;
            }
            t0++;
            pb = &D_800F4C38[t0];
        } while (t0 < D_800F4C38[0]);
    }
    *D_8010E690 = t2;
}
