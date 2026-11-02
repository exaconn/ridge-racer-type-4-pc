/* CC1_FLAGS: -mno-split-addresses */
typedef struct { char p0[0xC]; int fC; char p1[0x27]; unsigned char f37; } O;
extern int D_800ABCA8, D_800ABCB8, D_800ABCA4, D_800ABCE8;
extern int (*D_800ABC98)(), (*D_800ABC94)(), (*D_800ABC80)();
extern int func_80098788();
void func_8009970C(o) O *o; {
    if (D_800ABCA8 == D_800ABCB8) {
        if (D_800ABCA4 != 0) {
            D_800ABC98();
            D_800ABC94();
        }
    }
    if (D_800ABCE8 != 0) {
        D_800ABC80(o->fC);
        D_800ABC80(o->fC + 0xF0);
    }
    if (o->f37 == 0) func_80098788(o, 0x42); else func_80098788(o, o->f37);
}
