void func_80070170(int *a0, int a1) {
    int *p = a0 + 3;
    unsigned i = 0;
    a0[1] -= a1;
    a0[2] -= a1;
    if (a0[0] != 0) {
        do {
            i++;
            *p -= a1;
            p++;
        } while (i < (unsigned)a0[0]);
    }
}
