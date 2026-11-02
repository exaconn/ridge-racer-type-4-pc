extern void func_8006E030();
extern void func_8006E09C();
extern void func_8006E108();
extern void func_80090DB4();

void func_8006E300(int *a0) {
    char sp10[32];
    func_8006E09C(0x1F800108, a0[1]);
    func_8006E108(sp10, a0[0]);
    func_80090DB4(sp10, 0x1F800108);
    func_8006E030(sp10, a0[2]);
    func_80090DB4(sp10, 0x1F800108);
}
