extern int func_8002B330();
extern unsigned char D_8009F7E4[];
void func_800397BC(unsigned char *a0) {
    int i;
    for (i = 0; i < 0x29; i++) {
        if (*a0 == D_8009F7E4[i]) break;
    }
    func_8002B330(2, 4, &i, 0x29, 0);
    *a0 = D_8009F7E4[i];
}
