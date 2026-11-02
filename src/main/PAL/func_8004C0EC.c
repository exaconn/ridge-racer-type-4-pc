extern int func_8004FA98();
extern int func_80052B48();
extern int func_80053A9C();
extern short D_800AD55A;
extern char D_800F4DB8[];
void func_8004C0EC(void) {
    char *p = D_800F4DB8;
    func_80052B48(*(unsigned short *)(p + 0x26C4));
    func_80053A9C(*(short *)(p + 0x2888));
    func_8004FA98(D_800AD55A);
}
