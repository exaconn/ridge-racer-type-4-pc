extern int *D_800AC598;

void func_80070400(int *a0) {
    int *p = a0 + 1;
    int i = 0;
    int n = *a0 * 8;
    D_800AC598 = p;
    if (n > 0) {
        do {
            i++;
            *p += (int)a0;
            p++;
        } while (i < n);
    }
}
