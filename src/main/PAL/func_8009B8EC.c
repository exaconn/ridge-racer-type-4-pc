extern int func_8009A94C();
extern int func_8009A9A4();
extern int func_8009ABF8();
extern int func_8009AC50();
int func_8009B8EC(a0, a1)
    int *a0;
    int a1;
{
    int v;
    a0[6] = 0;
    if (a1 == 1) {
        if (a0[0] & 0x8000) {
            if (func_8009A94C(a0[2], a0[3]) != 0) {
                a0[6] = 0x10;
                return -1;
            }
            return 0;
        }
        v = func_8009A9A4(a0[2], a0[3]);
        if (v < 0) a0[6] = 5;
        return v;
    }
    if (a1 == 2) {
        if (a0[0] & 0x8000) {
            if (func_8009ABF8(a0[2], a0[3]) == 0) return 0;
            a0[6] = 0x10;
            return -1;
        }
        v = func_8009AC50(a0[2], a0[3]);
        if (v < 0) a0[6] = 5;
        return v;
    }
    a0[6] = 0x16;
    return -1;
}
