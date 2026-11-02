extern int func_8008B154(), func_8008C118(), func_8008C12C(), func_8008C27C(), func_8008C51C(), func_8008C53C(), func_8008C55C(), func_8008C580(), func_8008C6A4(), func_8008EB6C(), func_800963DC();
extern char D_80011D28[];
extern void (*D_800A63F0)();
extern int D_800A63F4;
extern int D_800A67B0[];
extern int D_800A67B8[];
extern int D_800A67D0;
extern char func_8008E7F4[], func_8008EA9C[];
void func_8008E828(a0, a1) int a0; int a1; {
    int sp10[4];
    int x;
    D_800A67B0[13] = a1;
    if ((a0 & 0xFF) == 1) {
        if (D_800A67B0[5] > 0) {
            if (D_800A67B0[4] == 0x200) {
                if (D_800A63F4 & 1) {
                    func_8008C55C(0);
                    func_8008C53C(sp10, 3);
                    func_8008C580(0);
                    func_8008C55C(func_8008EA9C);
                } else {
                    func_8008C51C(sp10, 3);
                }
                if (func_8008C6A4(sp10) != D_800A67D0) {
                    func_800963DC(D_80011D28);
                    D_800A67B0[5] = -1;
                }
            }
            if (D_800A63F4 & 1) {
                func_8008C53C(D_800A67B8[0], D_800A67B8[2]);
            } else {
                func_8008C51C(D_800A67B8[0], D_800A67B8[2]);
                D_800A67B0[2] = D_800A67B8[0] + D_800A67B8[2] * 4;
                D_800A67B0[5] = D_800A67B0[5] - 1;
                D_800A67B0[8] = D_800A67B0[8] + 1;
            }
        }
    } else {
        D_800A67B0[5] = -1;
    }
    D_800A67B0[6] = func_8008B154(-1);
    if (D_800A67B0[5] < 0) func_8008EB6C(1);
    if (D_800A67B0[7] + 0x4B0 < func_8008B154(-1)) D_800A67B0[5] = -1;
    if (D_800A67B0[5] != 0) {
        if (D_800A67B0[7] + 0x4B0 < func_8008B154(-1)) goto blk;
    } else {
    blk:
        func_8008C12C(D_800A67B0[11]);
        if (D_800A63F4 & 1) func_8008C55C(D_800A67B0[12]);
        func_8008C118(func_8008E7F4);
        func_8008C27C(9, 0);
        D_800A67B0[9] = 1;
        if (D_800A63F0 != 0) {
            x = 5;
            if (D_800A67B0[5] == 0) x = 2;
            D_800A63F0(x, a1);
        }
    }
}
