/* GCC_VERSION: 2.7.2 */
extern int *D_800ABBC4;
extern int *D_800ABBC8;
int func_80094AE8(int *a, int n) {
    int i = n - 1;
    *D_800ABBC8 = 0x04000000;
    if (n) {
        do { int v = *a; a++; i--; *D_800ABBC4 = v; } while (i != -1);
    }
    return 0;
}
