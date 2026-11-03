/* CC1_FLAGS: -mno-split-addresses */
extern unsigned char D_800ABAC2;
extern char D_80011F38[];
extern int (*D_800ABABC)();
extern struct { char p[0x18]; int (*f18)(); } *D_800ABAB8;
extern volatile unsigned *D_800ABBD4, *D_800ABBC8;
extern int D_800ABBFC, D_800ABC00;
extern int func_8008B154(), func_80095394(), func_8008B3C4(), func_80095990();
int func_80095894(a0) int a0; {
    if (D_800ABAC2 >= 2) D_800ABABC(D_80011F38, a0);
    D_800ABBFC = func_8008B154(-1) + 0xF0;
    D_800ABC00 = 0;
    while ((*D_800ABBD4 & 0x1000000) || (*D_800ABBC8 & 0x4000000) == 0) {
        if (func_80095394() != 0) return -1;
    }
    func_8008B3C4(2, func_80095990);
    D_800ABAB8->f18(a0);
    return 0;
}
