/* GCC_VERSION: 2.7.2 */
struct R { short a; short b; unsigned short c; short d; short e; unsigned short f; };
struct Q { int a, b; };
extern struct Q *D_800ABCC8;
extern struct R *D_800ABCCC;
extern int func_8009A814();
int func_80098A00() {
    D_800ABCC8->a = -0x81;
    if (D_800ABCCC->c & 0x80) {
        do {
            if (func_8009A814()) return 0;
        } while (D_800ABCCC->c & 0x80);
    }
    D_800ABCCC->f |= 0x10;
    return 1;
}
