extern int func_8004E194();
extern int func_8004E260();
extern int func_8004E414();
extern int D_800A0218;
extern int D_800A021C;
extern int D_800A0220;
extern int D_800A0224;
extern unsigned short D_800A02C0;
extern int D_800ACC18;
extern short D_800F3914;
extern int D_8010C1F0;

int func_8004E5E8(void) {
    switch (D_800A0218) {
    case 0:
        func_8004E414();
        D_800A0218 += 1;
    case 1:
        if (func_8004E194() != 0) {
            D_800A021C = 0;
            D_800A0220 = 0;
            D_800A0224 = 0;
            D_8010C1F0 = D_800ACC18;
            *(int *)&D_800A02C0 = D_800F3914;
            if (D_800F3914 >= 8) {
                D_800F3914 = 7;
            }
            D_800A0218 += 1;
        }
        return 0;
    case 2:
        if (func_8004E260() != 0) {
            D_800A0218 += 1;
            D_800F3914 = D_800A02C0;
        }
        return 0;
    case 3:
        return 1;
    default:
        return 0;
    }
}
