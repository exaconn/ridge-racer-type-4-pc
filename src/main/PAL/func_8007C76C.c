extern int func_8007CFC0();
extern short D_800F3A1C;
extern short D_800F4D3C;
extern unsigned short D_800F4D8A;
extern short D_800FFC18;
extern int D_801272F0[];

void func_8007C76C()
{
    short t;
    if (D_800F4D8A == 0) {
        D_801272F0[0] = -1;
        D_801272F0[1] = -1;
        D_801272F0[2] = -1;
        D_800F4D3C = 0;
        func_8007CFC0(0, 0);
        D_800F4D8A = 1;
        return;
    }
    if (func_8007CFC0(1, D_800F4D3C) != 0) {
        t = (unsigned short)D_800F4D3C + 1;
        D_800F4D3C = t;
        if (t == 3) {
            if (D_800FFC18 == 7) {
                D_800F3A1C = 0x2B;
            } else {
                D_800F3A1C = 0x2A;
            }
            D_800F4D3C = 0;
            D_800F4D8A = 0;
            return;
        }
        func_8007CFC0(0, t);
    }
}
