struct S { int p[50]; };
extern struct S D_8010B2E0[];
int func_800172DC(void) {
    int i;
    for (i = 0; i < 16; i++) {
        D_8010B2E0[i].p[3] = 0;
        D_8010B2E0[i].p[0xD] = 0;
        D_8010B2E0[i].p[0x17] = 0;
        D_8010B2E0[i].p[0x1A] = 0;
    }
}
