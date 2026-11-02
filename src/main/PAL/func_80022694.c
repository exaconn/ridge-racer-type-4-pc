extern int func_8001FCC8();
extern int func_80020588();
extern int func_8002064C();
extern int D_800ACC14;
void func_80022694(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
    int d1, d2, x, y, r, t;
    if (a5 > 0) {
        d1 = a3 - a1;
        d2 = a4 - a2;
        if (a5 >= 0x100) a5 = 0x100;
        r = *(int *)0x1F800000;
        x = a1 + ((d1 * a5) >> 8);
        y = a2 + ((d2 * a5) >> 8);
        if (a10 >= 0) {
            r = func_8002064C(a0, r, a1, a2, x, y, a7, a8, a9);
            if (a6 != 0) {
                r = func_8002064C(a0, r, a1, a2 + 1, x, y + 1, a7, a8, a9);
            }
            r = func_8001FCC8(D_800ACC14 + 0x70, r, ((a10 & 3) << 5) | 5);
        } else {
            r = func_80020588(a0, r, a1, a2, x, y, a7, a8, a9);
            if (a6 != 0) {
                r = func_80020588(a0, r, a1, a2 + 1, x, y + 1, a7, a8, a9);
            }
        }
        *(int *)0x1F800000 = r;
    }
}
