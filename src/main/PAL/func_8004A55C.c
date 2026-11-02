extern int func_80096434();
extern int func_80097854();
extern int func_80099CD4();
extern int D_800F3A38[];
extern char D_8010C170[];
extern char D_8010C1A0[];
void func_8004A55C(void) {
    func_80096434(D_800F3A38, 0xB8);
    func_80096434(D_8010C170, 0x30);
    D_800F3A38[4] = -1;
    D_800F3A38[27] = -1;
    func_80099CD4(D_8010C1A0, D_8010C1A0 + 0x28);
    func_80097854();
}
