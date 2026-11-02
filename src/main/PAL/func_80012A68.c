void func_80012A68(int *a0)
{
    int n = *a0;
    int *p = a0 + 1;
    int t;
    if (n-- > 0) {
        int *q = a0 + 3;
        do {
            t = n--;
            *p += (int)a0;
            p += 3;
            *q += (int)a0;
            q += 3;
        } while (t > 0);
    }
}
