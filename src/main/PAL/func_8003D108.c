extern int D_800ACC24;
extern char *D_800F2AB0;
extern int D_800F2ECC;
extern char *D_800F3AFC;
int func_8003D108(a0) int a0;
{
    int r = 3;
    int i;
    int n;
    char *p;
    if (a0 & 1) {
        i = 0;
        n = D_800F2ECC;
        if (n > 0) {
            int c = 0xB;
            int m = n;
            p = D_800F3AFC;
            do {
                short t = *(short *)(p + 0x10);
                if (t != 0 && t != c) {
                    r &= 0xFFFE;
                    break;
                }
                i++;
                p += 0x1C;
            } while (i < m);
        }
    }
    if (a0 & 2) {
        i = 0;
        n = D_800ACC24;
        if (n > 0) {
            int c = 5;
            int m = n;
            a0 = (int)D_800F2AB0;
            do {
                signed char t = *(char *)(a0 + 1);
                if (t != 0) {
                    i++;
                    if (t != c) {
                        r &= 0xFFFD;
                        break;
                    }
                } else i++;
                a0 += 0x14;
            } while (i < m);
        }
    }
    return r;
}
