/* GCC_VERSION: 2.7.2 */
extern char D_801144D8[];
extern int D_800ABCC0[];
extern int D_800ABCA8, D_800ABCAC, D_800ABCBC;
extern char *D_800ABD00;
extern int func_8009A6DC(), func_80099F90(), func_80098154();
void func_80099E9C(a0) int a0; {
    register int a1 __asm__("$5"); /* ASM_BARRIER: original keeps the selector copy in a1 (a0 is reused for call arguments) */
    int idx;
    char *p;
    char *q;
    int done;
    int c;
    char *base;
    a1 = a0;
    base = D_801144D8;
    c = -9;
    do {
        idx = D_800ABCA8;
        p = (char *)(idx * 0xF0 + (int)base);
        if (a1 != c) {
            if (a1 == 0) {
                D_800ABCC0[idx] = 0;
            } else {
                func_8009A6DC(p);
                func_80099F90(p);
            }
        }
        q = D_800ABD00;
        idx = D_800ABCA8;
        D_800ABCAC = 0;
        *(short *)(q + 0xA) = 0;
        idx++;
        D_800ABCA8 = idx;
        if (D_800ABCBC < idx) {
            done = 1;
            a1 = 0xFFFF;
        } else {
            done = func_80098154((char *)(idx * 0xF0 + (int)base));
            a1 = 0xFFFF;
        }
    } while (done == 0);
}
