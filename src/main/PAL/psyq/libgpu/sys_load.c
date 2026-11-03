/* CC1_FLAGS: -mno-split-addresses */
extern int func_8008B154();
extern void func_8008B3C4();
extern void func_80092DB8();
extern void func_80095990();
extern int func_80095394();
extern char D_80011FCC[];
extern char *D_800ABAB8;
extern volatile int *D_800ABBC8;
extern volatile int *D_800ABBD4;
extern int D_800ABBFC;
extern int D_800ABC00;
int func_80095578(int a0, int a1) {
    func_80092DB8(D_80011FCC, a0);
    D_800ABBFC = func_8008B154(-1) + 0xF0;
    D_800ABC00 = 0;
    while ((*D_800ABBD4 & 0x01000000) || !(*D_800ABBC8 & 0x04000000)) {
        if (func_80095394() != 0) return -1;
    }
    func_8008B3C4(2, func_80095990);
    (*(void (**)())(D_800ABAB8 + 0x20))(a0, a1);
    return 0;
}
