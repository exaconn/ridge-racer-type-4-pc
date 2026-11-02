struct B8 { unsigned char c[8]; };
extern struct B8 D_80010104;
extern int func_8002A344();
extern int func_8002AF8C();
extern int func_8003A1B4();
extern int func_80092F64();
extern short D_800AC5D0;
extern int D_800AC5DC;
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int D_8010BF74;
void func_80019F20(void) {
    struct B8 sp10;
    D_800AC5DC = 0x80;
    func_8003A1B4();
    func_8002AF8C(0, 1, 0);
    func_8002A344(0x1000C);
    sp10 = D_80010104;
    func_80092F64(&sp10, 0, 0, 0);
    D_800AC5D0 = 0;
    D_8010BF74 = 0;
    D_800F4C6A = 0;
    D_800F3A26 = 4;
    D_800F4D8A = 0;
}
