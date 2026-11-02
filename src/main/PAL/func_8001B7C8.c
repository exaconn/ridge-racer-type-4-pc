void func_8001B7C8(unsigned short *a0, unsigned short *a1, short *a2) {
    int v = *a2;
    unsigned short u = *(unsigned short *)a2;
    if (v > 0) {
        *a1 += u;
        if ((short)*a1 >= 0x1000) {
            *a1 = 0x1000;
            *a2 = 0;
        }
    } else if (v < 0) {
        *a0 += u;
        if ((short)*a0 <= 0) {
            *a0 = 0;
            *a2 = 0;
        }
    }
}
