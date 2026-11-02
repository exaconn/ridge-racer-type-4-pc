/* GCC_VERSION: 2.7.2 */
extern unsigned short *D_800A47D8;
extern int D_800A4800;
void func_8007F3D0(int a0, unsigned int a1, int a2) {
    if (a2 == 0) {
        D_800A47D8[a0] = a1;
    } else {
        D_800A47D8[a0] = a1 >> D_800A4800;
    }
}
