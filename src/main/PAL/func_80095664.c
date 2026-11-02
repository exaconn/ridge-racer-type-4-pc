/* CC1_FLAGS: -mno-split-addresses */
extern char D_80011EF0[];
extern volatile unsigned int *D_800ABBD4;
extern volatile unsigned int *D_800ABBC8;
extern int D_800ABBFC;
extern int D_800ABC00;
extern int *D_800ABAB8;
extern int func_80092DB8();
extern int func_8008B154();
extern int func_8008B3C4();
extern int func_80095394();
extern int func_80095990();
int func_80095664(a0, a1)
int a0;
int a1;
{
    func_80092DB8(D_80011EF0, a0);
    D_800ABBFC = func_8008B154(-1) + 0xF0;
    D_800ABC00 = 0;
    while ((*D_800ABBD4 & 0x1000000) || (*D_800ABBC8 & 0x4000000) == 0) {
        if (func_80095394()) return -1;
    }
    func_8008B3C4(2, func_80095990);
    ((int (*)())D_800ABAB8[7])(a0, a1);
    return 0;
}
