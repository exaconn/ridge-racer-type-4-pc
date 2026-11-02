extern int func_80096434();
void func_8004CBDC(char *p) {
    func_80096434(p, 0x2C);
    *(int *)(p + 4) = 1;
    p[0x10] = 0x80;
    p[0x11] = 0x80;
    p[0x12] = 0x80;
}
