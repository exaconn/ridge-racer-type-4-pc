/* GCC_VERSION: 2.7.2 */
extern unsigned char *D_800A674C, *D_800A6750, *D_800A6754, *D_800A6758;
int func_8008D5D4(unsigned char *a) {
    *D_800A674C = 2;
    *D_800A6754 = a[0];
    *D_800A6758 = a[1];
    *D_800A674C = 3;
    *D_800A6750 = a[2];
    *D_800A6754 = a[3];
    *D_800A6758 = 0x20;
    return 0;
}
