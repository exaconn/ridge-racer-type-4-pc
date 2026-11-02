extern int func_80086B84();
int func_80086A54(a0, a1, a2) short a0; short *a1; short *a2; {
    short sp[2];
    if ((unsigned short)a0 < 0x18) {
        func_80086B84(a0, &sp[0], &sp[1]);
        *a1 = sp[0] / 129;
        *a2 = sp[1] / 129;
        return 0;
    }
    return -1;
}
