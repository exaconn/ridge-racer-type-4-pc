extern char D_8009F014[];
char *func_8002BB88(a) int a; {
    if (a >= 0) {
        int s = a / 25;
        int m = s / 60;
        int r = s % 60;
        D_8009F014[0] = m / 10 + 0x30;
        D_8009F014[1] = m % 10 + 0x30;
        D_8009F014[3] = r / 10 + 0x30;
        D_8009F014[4] = r % 10 + 0x30;
    } else {
        D_8009F014[0] = 0x2D; D_8009F014[1] = 0x2D; D_8009F014[3] = 0x2D; D_8009F014[4] = 0x2D;
    }
    return D_8009F014;
}
