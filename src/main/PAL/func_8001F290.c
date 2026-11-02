extern unsigned char D_800F2FE0[];
extern unsigned char D_800F3B28[];
void func_8001F290(void) {
    int i;
    for (i = 0; i < 0x840; i++) {
        D_800F2FE0[i] = D_800F3B28[i];
    }
}
