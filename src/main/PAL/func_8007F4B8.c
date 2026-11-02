/* GCC_VERSION: 2.7.2 */
extern unsigned short *D_800A47D8;
extern int D_800A4800;
int func_8007F4B8(int a0, int a1) {
    unsigned short v = D_800A47D8[a0];
    if (a1 == -1) return v;
    return v << D_800A4800;
}
