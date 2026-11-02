extern int D_80110348, D_80112754;
extern char D_80112750[];
extern int func_8008C6A4(), func_8008C5A0();
int func_8008F670(int a) {
    int r;
    if (D_80110348 == 0) {
        func_8008C5A0(func_8008C6A4(D_80112750) + 1, a);
        r = D_80112754;
    } else {
        r = -1;
    }
    return r;
}
