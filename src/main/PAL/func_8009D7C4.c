/* GCC_VERSION: 970318 */
/* RODATA 0x800121a0 0x3c */
extern int D_800ABDF8;
extern int D_80114730;
extern char *D_80114718;
int func_8009D7C4(a0) int a0; {
    int v;
    int r;
    char *c;
    switch (D_800ABDF8) {
    case 3:
        D_80114730 = a0 & 0xFF;
        break;
    case 4:
        D_80114718[1] = a0;
        break;
    case 5:
        D_80114718[0] = a0;
        break;
    case 6:
        D_80114718[0x10] = a0;
        break;
    case 17:
        D_80114718[0xC] = a0 - 1;
        break;
    default:
        v = D_800ABDF8;
        if ((unsigned)(v - 9) < 4) {
            c = D_80114718 + 0x10;
            c -= v;
            *c = a0;
            break;
        }
        if (v < 7) break;
        D_80114718[v - 5] = a0;
        break;
    }
    r = 0;
out:
    return r;
}
