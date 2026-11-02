/* GCC_VERSION: 2.7.2 */
extern int D_800ABCB4;
extern char *D_800ABC9C;
int func_80097894(a0) int a0; {
    if (D_800ABCB4) {
        int i = a0 >> 4;
        return D_800ABC9C[i * 0xF0 + 0xE8] == 8;
    }
    return 0;
}
