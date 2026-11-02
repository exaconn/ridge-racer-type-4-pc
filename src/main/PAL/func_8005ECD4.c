extern int D_800ACA28;
extern int D_800A1CD0[];
extern void func_80096434();

void func_8005ECD4(int a0) {
    int *p = (int *)(D_800ACA28 + 0x134);
    func_80096434(p, 0x28);
    p[1] = 0;
    p[5] = D_800A1CD0[a0];
}
