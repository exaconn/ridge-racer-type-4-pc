/* GCC_VERSION: 2.7.2 */
extern int D_801102FC;
extern int D_80110314;
extern char *D_80110310;
int func_8008F764(int a) {
    int i, j, k;
    char *t;
    unsigned short m;
    i = ((a - ((int)D_80110310 + (D_80110314 << 5))) >> 2) / 504;
    t = D_80110310 + (i << 5);
    m = *(unsigned short *)(t + 6);
    if (*(short *)t == 4) {
        j = 0;
        if ((short)m > 0) {
            do { k = j + i; j++; *(short *)(D_80110310 + (k << 5)) = 0; } while (j < (short)m);
        }
        D_801102FC = j + i;
        return 0;
    }
    return 1;
}
