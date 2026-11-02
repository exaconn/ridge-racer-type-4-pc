int func_8004D4BC(int a) {
    int v = a;
    int r = v & 3;
    if (r != 0) {
        v += 4 - r;
    }
    return v;
}
