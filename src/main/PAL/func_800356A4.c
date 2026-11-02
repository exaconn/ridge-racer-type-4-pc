extern int func_80096F44(), func_80096F54();
extern short D_800AD508;
extern int D_800F2AA8, D_800F2AAC;
int func_800356A4(a0, a1, a2, a3) int a0, a1, a2, a3;
{
    int r;
    if (D_800AD508 != 0) {
        r = -1;
        if (func_80096F54(D_800F2AAC) == a2) {
            if (a3 != 0) {
                if (func_80096F44(D_800F2AA8, a0, a2) == a2) goto zero;
neg:
                return -1;
            }
            goto zero;
        }
        return r;
    }
    if (func_80096F44(D_800F2AA8, a1, a2) != a2) goto neg;
    if (a3 != 0) {
        r = -1;
        if (func_80096F54(D_800F2AAC, a0, a2) != a2) return r;
    }
zero:
    return 0;
}
