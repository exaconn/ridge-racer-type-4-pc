/* CC1_FLAGS: -mno-split-addresses */
extern signed char *D_800A674C;
extern signed char *D_800A6754;
extern unsigned char *D_800A6758;
extern int *D_800A675C;
extern unsigned char D_800A6764[];
void func_8008D65C() {
    volatile unsigned char *p;
    *D_800A674C = 1;
    if (*D_800A6758 & 7) {
        do {
            *D_800A674C = 1;
            *D_800A6758 = 7;
            *D_800A6754 = 7;
        } while (*D_800A6758 & 7);
    }
    p = D_800A6764;
    p[2] = 0;
    p[1] = p[2];
    p[0] = 2;
    *D_800A674C = 0;
    *D_800A6758 = 0;
    *D_800A675C = 0x1325;
}
