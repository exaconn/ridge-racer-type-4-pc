/* GCC_VERSION: 2.7.2 */
extern int func_8008A564();
extern unsigned char D_80110120[];
extern signed char D_80110107;
extern unsigned char *D_801100F8;
int func_800850C4(a0, a1, a2, a3)
short a0; short a1; int a2; unsigned char *a3;
{
    int o;
    unsigned char *s;
    if (D_80110120[a0] == 1) {
        func_8008A564(a0, a1);
        o = (short)(a2 + D_80110107 * 16) << 5;
        a3[0] = ((unsigned char *)(o + (int)D_801100F8))[0];
        a3[1] = ((unsigned char *)(o + (int)D_801100F8))[1];
        a3[2] = ((unsigned char *)(o + (int)D_801100F8))[2];
        a3[3] = ((unsigned char *)(o + (int)D_801100F8))[3];
        a3[4] = ((unsigned char *)(o + (int)D_801100F8))[4];
        a3[5] = ((unsigned char *)(o + (int)D_801100F8))[5];
        a3[7] = ((unsigned char *)(o + (int)D_801100F8))[7];
        a3[6] = ((unsigned char *)(o + (int)D_801100F8))[6];
        a3[8] = ((unsigned char *)(o + (int)D_801100F8))[8];
        a3[9] = ((unsigned char *)(o + (int)D_801100F8))[9];
        a3[10] = ((unsigned char *)(o + (int)D_801100F8))[10];
        a3[11] = ((unsigned char *)(o + (int)D_801100F8))[11];
        a3[12] = ((unsigned char *)(o + (int)D_801100F8))[12];
        a3[13] = ((unsigned char *)(o + (int)D_801100F8))[13];
        s = (unsigned char *)(o + (int)D_801100F8);
        *(unsigned short *)(a3 + 16) = *(unsigned short *)(s + 16);
        *(unsigned short *)(a3 + 18) = *(unsigned short *)(s + 18);
        *(unsigned short *)(a3 + 20) = *(unsigned short *)(s + 20);
        *(unsigned short *)(a3 + 22) = *(unsigned short *)(s + 22);
        return 0;
    }
    return -1;
}
