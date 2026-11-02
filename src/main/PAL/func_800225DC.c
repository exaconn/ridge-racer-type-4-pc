extern int func_8001FCC8();
extern int func_8001FF84();
extern char D_8009EE6C[];
extern char *D_800ACC14;
extern int D_800FF6B0;
void func_800225DC(int a0) {
    int a3;
    char *s0;
    void *p;
    if ((unsigned)(D_800FF6B0 - 1) < 3U) {
        p = *(void **)0x1F800000;
        if (a0 > 0) {
            a3 = ((a0 - 1) << 7) + 0x38;
            if (a0 == 1) {
                s0 = D_800ACC14 + 0x70;
            } else {
                s0 = D_800ACC14 + 0xB70;
            }
        } else {
            a3 = 0x78;
            s0 = D_800ACC14 + 0x70;
        }
        *(int *)0x1F800000 = func_8001FCC8(s0, func_8001FF84(s0, p, 0x6A, a3, D_8009EE6C, 0x7EF8, 0), 0x3E);
    }
}
