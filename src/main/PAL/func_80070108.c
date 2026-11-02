extern int *D_8010E698[];

void func_80070108(int *a0, int a1) {
    int *p = a0 + 3;
    unsigned i;
    D_8010E698[a1] = a0;
    i = 0;
    a0[1] += (int)a0;
    a0[2] += (int)a0;
    if (a0[0] != 0) {
        do {
            i++;
            *p += (int)a0;
            p++;
        } while (i < (unsigned)a0[0]);
    }
}
