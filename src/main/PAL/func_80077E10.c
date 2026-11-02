extern int func_80095D04();
extern int func_80095D70();
extern int func_80095C74();
extern int func_8009BA04();
extern int func_80096F04();
extern int func_80096E74();
extern int func_80096F14();
extern int func_80096EB4();
extern int D_8010EF58, D_8010EF5C, D_8010EF60, D_8010EF64, D_8010EF68, D_8010EF6C, D_8010EF70, D_8010EF74;
extern unsigned char D_800F3A24[];
extern int D_800ACC0C;
extern char D_800FFC48[];

void func_80077E10()
{
    func_80095D04();
    func_80095D70();
    func_80095C74();
    func_8009BA04();
    func_80096F04();
    D_8010EF58 = func_80096E74(0xF4000001, 4, 0x2000, 0);
    D_8010EF5C = func_80096E74(0xF4000001, 0x8000, 0x2000, 0);
    D_8010EF60 = func_80096E74(0xF4000001, 0x100, 0x2000, 0);
    D_8010EF64 = func_80096E74(0xF4000001, 0x2000, 0x2000, 0);
    D_8010EF68 = func_80096E74(0xF0000011, 4, 0x2000, 0);
    D_8010EF6C = func_80096E74(0xF0000011, 0x8000, 0x2000, 0);
    D_8010EF70 = func_80096E74(0xF0000011, 0x100, 0x2000, 0);
    D_8010EF74 = func_80096E74(0xF0000011, 0x2000, 0x2000, 0);
    func_80096F14();
    func_80096EB4(D_8010EF58);
    func_80096EB4(D_8010EF5C);
    func_80096EB4(D_8010EF60);
    func_80096EB4(D_8010EF64);
    func_80096EB4(D_8010EF68);
    func_80096EB4(D_8010EF6C);
    func_80096EB4(D_8010EF70);
    func_80096EB4(D_8010EF74);
    D_800F3A24[1] = 0x5A;
    D_800F3A24[0] = 0x5A;
    D_800ACC0C = (int)D_800FFC48;
}
