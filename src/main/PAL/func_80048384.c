extern char *D_800ABEA4;
extern int D_800ABECC;

int func_80048384(int a0, int a1) {
    if (a1 > 0) {
        int n = D_800ABECC;
        char *p = D_800ABEA4;
        do {
            a0--;
            if (a0 < 0) a0 += n;
            a1 -= *(short *)(p + a0 * 60 + 0x1C);
        } while (a1 > 0);
    }
    return a0;
}
