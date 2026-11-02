extern char D_800ABF48[];
extern int D_8010E690;
extern short D_800F3914;
extern void func_8006E834();
extern void func_80071348();
extern void func_8006EC64();
extern void func_8006E9E8();
extern void func_8006EAC8();
extern void func_80060C70();

void func_8006ECE8(int ignoredPlayer) {
    char *p = D_800ABF48;
    func_8006E834();
    func_80071348();
    func_8006EC64();
    func_8006E9E8(p, p - 0x30);
    func_8006EAC8();
    func_80060C70(0x1F800000, D_8010E690, D_800F3914);
}
