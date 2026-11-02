extern unsigned func_8009AEAC();
extern short D_800FF688;
int func_800353E4(void) {
    int r = 0;
    int t = (func_8009AEAC(0, 0, 0) >> 7) & 3;
    if (t == 0 || t == 3) {
        D_800FF688 = -1;
        r = -1;
    } else if (t == 1) {
        D_800FF688 = t;
    } else if (t == 2) {
        if (D_800FF688 == 3) r = 1;
    }
    return r;
}
