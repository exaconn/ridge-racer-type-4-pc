extern int func_8002A344();
extern int func_8002AF8C();
extern int func_8002CC88();
extern int func_8002E5A0();
extern int func_8002E828();
extern int func_80039988();
extern int func_8003FBA8();
extern int func_80052B8C();
extern int func_80053BB4();
extern int func_800578D8();
extern int func_8006E1AC();
extern int func_8006EF8C();
extern unsigned char D_8009F818[];
extern char D_800ABEF8[];
extern short D_800AC5D0, D_800F3A26, D_800F4C6A;
extern signed char D_800AD4EF;
extern signed char *D_800F2AB0;
extern int D_800F2DE4;
extern unsigned short D_800F4D8A;
void func_80039ABC(void) {
    D_800F2DE4 += 1;
    func_8006EF8C(6);
    if (D_800F4D8A == 0) {
        if (func_800578D8(D_8009F818[D_800AD4EF / 2]) != 0) {
            D_800AC5D0 = 0x40;
            func_80052B8C(0, 0x3C);
            D_800F4D8A = 1;
            func_80039988();
            D_800F2AB0[1] = 0;
        }
    } else {
        func_8006E1AC();
        func_8002E828(2, 0x1F800028);
        func_8002E5A0(D_800ABEF8);
        func_8002CC88(D_800ABEF8, 0, 0x100, 1);
        func_8003FBA8(0);
        if (D_800F2AB0[1] == 2) {
            D_800F4C6A = 1;
            D_800F3A26 = 0x22;
            D_800F4D8A = 0;
            func_80053BB4(0);
        }
    }
    func_8002A344(0x10028);
    func_8002AF8C(0, 0, 0);
}
