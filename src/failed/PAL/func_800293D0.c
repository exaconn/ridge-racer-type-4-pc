struct Q { int a, b, c, d; };
extern int *D_800AC570[];
extern short D_800AC59C;
extern int D_800AC5A8, D_800AD504, D_800ADAC8, D_80107188;
extern char *D_800AD510[];
extern int *D_800FFC20[];
extern char D_800FFC48[];
void func_800293D0(a0, a1) int a0; int a1; {
    struct Q *src;
    struct Q *dst;
    struct Q *end;
    int **p2;
    int **p3;
    char **p4;
    int t;
    int k;
    char *u;
    int j;
    D_800AC5A8 = 0;
    D_800AD504 = a1;
    if (a1 == 2) {
        a1 = 0;
        if (D_800AC59C > 0) {
            p2 = D_800FFC20;
            p3 = D_800AC570;
            do {
                src = (struct Q *)*p3;
                dst = (struct Q *)*p2;
                end = src + 50;
                do {
                    *dst++ = *src++;
                } while (src != end);
                if (*(short *)((char *)*p2 + 0x1E4) > 0x10)
                    *(short *)((char *)*p2 + 0x1E4) &= 0xFF;
                p2++;
                a1++;
                p3++;
            } while (a1 < D_800AC59C);
        }
        return;
    }
    D_800ADAC8 = 0;
    a1 = 0;
    if (a0 == 1) {
        j = 0x5468;
        D_80107188 = 0xA8C;
        D_800AD510[0] = D_800FFC48;
        D_800AD510[1] = D_800FFC48 + 0x2A34;
        p3 = D_800AC570;
        do {
            *p3 = (int *)(j + D_800FFC48);
            p3++;
            a1++;
            j += 0x320;
        } while (a1 < 4);
        return;
    }
    if (a0 == 2) {
        D_80107188 = 0x384;
        k = 0x385;
        a1 = 0;
        p3 = D_800AC570;
        j = 0x3850;
        p4 = D_800AD510;
        do {
            t = k * (a1 * 4);
            u = j + D_800FFC48;
            j += 0x320;
            a1++;
            *p4 = D_800FFC48 + t;
            *p3 = (int *)u;
            p3++;
            p4++;
        } while (a1 < 4);
        return;
    }
    p3 = D_800AC570;
    j = 0x3844;
    D_80107188 = 0xE10;
    D_800AD510[0] = D_800FFC48;
    do {
        *p3 = (int *)(j + D_800FFC48);
        p3++;
        a1++;
        j += 0x320;
    } while (a1 < 8);
}
