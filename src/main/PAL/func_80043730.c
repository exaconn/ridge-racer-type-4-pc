extern int D_800ABECC;
extern char *D_800ACBA0;
int func_80043730(a0) char *a0;
{
    int t2 = 0;
    int t1 = 0x7FFFFFFF;
    char *a2 = D_800ACBA0;
    unsigned short a1 = *(unsigned short *)(a2 + 0x32);
    int a3 = 0;
    int a0v, v1, b;
    if (*(short *)(a2 + 0x32) != -1) {
        a0v = *(int *)(a0 + 0xF0);
        do {
            v1 = (short)a1;
            if (v1 < a0v) v1 = a0v - v1; else v1 = v1 - a0v;
            b = v1;
            if (D_800ABECC / 2 < v1) b = D_800ABECC - v1;
            v1 = b;
            if (v1 < t1) { t2 = a3; t1 = v1; }
            a2 += 0x44;
            a1 = *(unsigned short *)(a2 + 0x32);
            a3++;
        } while (*(short *)(a2 + 0x32) != -1);
    }
    return t2;
}
