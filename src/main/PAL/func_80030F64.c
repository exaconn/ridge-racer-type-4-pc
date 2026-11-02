extern int func_8001FD20();
extern int func_80090424();
extern unsigned char D_8009F1EC[];
extern int D_800ACC14;
extern signed char D_800AD4EF;
extern int D_800F2DE4;

int func_80030F64(int a0, int a1) {
    int s0 = 0x28;
    int sel = D_800AD4EF;
    int i = 0;
    int v, y;
    do {
        s0 += D_8009F1EC[i];
        if (sel == i) {
            v = func_80090424((D_800F2DE4 << 6) & 0xFC0) / 64 + 0xBF;
        } else if (i < sel) {
            v = 0x80;
        } else {
            s0 += 8;
            goto next;
        }
        y = s0;
        s0 += 8;
        a1 = func_8001FD20(D_800ACC14 + 0x70, a1, y, 0x190, 8, 0x1C, v, v, v);
next:
        i++;
    } while (i < 8);
    return a1;
}
