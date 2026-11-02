extern int func_8002A344();
extern int func_8002AF8C();
extern int func_8003B588();
extern int func_8003D108();
extern int func_8003D228();
extern int func_8003D6F0();
extern int func_8003DC9C();
extern int func_8003EC88();
extern int func_8003FAE4();
extern int func_8003FBA8();
extern char *D_800F2AB0;
extern unsigned int D_800F2DE4;
extern short D_800F3A26;
extern short *D_800F3AFC;
extern short D_800F4C6A;
extern int D_800F4CA8;
extern unsigned char *D_800F4D80;
extern short D_800F4D8A;
void func_80031064(void)
{
    int t;
    if (D_800F2DE4 <= 0x7FFF) {
        D_800F2DE4 += 1;
    }
    func_8002A344(0x1000B);
    if (D_800F3AFC[8] >= 4) {
        func_8003D6F0(0);
    }
    if (D_800F4D80[0x14] >= 3) {
        func_8003EC88(0);
    }
    if (((signed char *)D_800F2AB0)[1] >= 3) {
        func_8003FBA8(0);
    }
    if (D_800F2DE4 >= 3) {
        D_800F4CA8 = 1;
        t = func_8003B588();
        if (t == 1) {
            if (func_8003D108(3) == 3) {
                D_800F2DE4 = 0;
                func_8003D228(0, 0xC, 0x130, 0x128, 0x6C, 0, 0);
                func_8003DC9C(0, 0x10, 0x48, 0x120, 0x120, 0, 0);
                func_8003FAE4(0, -1, 0, 0);
                D_800F4C6A = t;
                D_800F3A26 = 0xC;
                D_800F4D8A = 0;
            }
        }
    } else {
        func_8002AF8C(0, 1, 0);
    }
}
