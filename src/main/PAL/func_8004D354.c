void func_8004D354(int *t, int a, int n, int b) {
    int h = n / 2;
    int x = a << 12;
    int z;
    int y = b << 12;
    t[2] = y;
    z = x / h - (b << 11);
    t[0] = x;
    t[1] = n;
    t[3] = z;
    t[4] = (z - t[2]) / h;
    t[5] = -z / h;
}
