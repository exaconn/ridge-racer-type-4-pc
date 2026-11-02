/* GCC_VERSION: 2.7.2 */
extern int func_80082674();
void func_800825B4(signed char a0, signed char a1, signed char a2) {
    struct { int x; int p[4]; int v24, v28; int pad; int v30, v34; } s;
    if (a0 == 0) {
        if (a1 == 0) { s.x = 0x200; s.v28 = a2; }
        if (a1 == 1) { s.x = 0x100; s.v24 = a2; }
    }
    if (a0 == 1) {
        if (a1 == 0) { s.x = 0x2000; s.v34 = a2; }
        if (a1 == 1) { s.x = 0x1000; s.v30 = a2; }
    }
    func_80082674(&s);
}
