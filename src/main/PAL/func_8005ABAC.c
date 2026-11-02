extern int D_800ACA28;
extern char D_800A1CE0[];
extern void func_80096434();

void func_8005ABAC(int a0) {
    int *p = (int *)(D_800ACA28 + 0x15C);
    func_80096434(p, 0x2C);
    p[1] = 0;
    p[3] = (int)(D_800A1CE0 + (a0 << 5));
    p[7] = 0x1000;
}
