extern short D_800AC0A4;
extern unsigned short D_800AD598[];
extern char D_800FF690[];
extern char D_800FF6B8[];
extern char D_800FF6D8[];
extern void func_8006E030();
extern void func_8006E09C();
extern void func_8006E108();
extern void func_8006E174();
extern void func_80090914();
extern void func_80090DB4();
extern void func_800910B4();

void func_8006E1AC(void) {
    char sp10[32];
    int *g = (int *)0x1F800000;
    func_8006E09C(0x1F800028, g[7]);
    func_8006E09C(D_800FF690, g[7]);
    func_8006E174(D_800FF6B8);
    func_8006E09C(D_800FF6D8, g[7]);
    func_8006E108(sp10, g[6]);
    func_80090DB4(sp10, 0x1F800028);
    func_80090DB4(sp10, D_800FF690);
    func_80090DB4(sp10, D_800FF6B8);
    func_8006E030(sp10, g[8]);
    func_80090DB4(sp10, 0x1F800028);
    func_80090DB4(sp10, D_800FF690);
    func_80090DB4(sp10, D_800FF6B8);
    func_8006E09C(sp10, 0x800);
    func_80090914(sp10, 0x1F800028, D_800AD598);
    func_8006E108(sp10, D_800AC0A4 * 2);
    func_80090DB4(sp10, D_800AD598);
    D_800AD598[0] = -D_800AD598[0];
    D_800AD598[1] = -D_800AD598[1];
    D_800AD598[2] = -D_800AD598[2];
    func_800910B4(0x1F800028);
}
