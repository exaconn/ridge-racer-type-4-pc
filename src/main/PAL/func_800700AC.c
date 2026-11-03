extern int D_800F2EC0;

void func_800700AC(int *a0) {
    int *a1 = a0 + 1;
    int i = 0;
    int n = a0[0];
    int lim;
    int *p;
    *(int *)0x1F800048 = (int)a1;
    D_800F2EC0 = n;
    if (n > 0) {
        lim = n;
        p = a0 + 3;
        do {
            i++;
            *a1 += (int)a0;
            a1 += 3;
            *p += (int)a0;
            p += 3;
        } while (i < lim);
    }
}
