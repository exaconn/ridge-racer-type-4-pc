/* GCC_VERSION: 2.7.2 */
extern unsigned int *D_800A47E8;
void func_8007F4F4(int a0) {
    *D_800A47E8 &= 0xFFF8FFFF;
    if (a0) {
        *D_800A47E8 |= 0x30000;
    } else {
        *D_800A47E8 |= 0x50000;
    }
}
