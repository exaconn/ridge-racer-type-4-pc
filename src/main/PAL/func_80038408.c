extern int func_80091734();
unsigned func_80038408(int a0, int a1, int a2, int a3, int a4) {
    unsigned r = 0;
    if (func_80091734(a0, a1, a4) >= 0) {
        if (func_80091734(a1, a3, a4) >= 0) {
            if (func_80091734(a3, a2, a4) >= 0) {
                r = (unsigned)~func_80091734(a2, a0, a4) >> 31;
            }
        }
    }
    return r;
}
