/* GCC_VERSION: 2.7.2 */
extern int func_80082674();
void func_800829F4(short a, short b) {
    struct { int x; short y, z; int pad[8]; } s;
    s.x = 3; s.y = a * 0x81; s.z = b * 0x81;
    func_80082674(&s);
}
