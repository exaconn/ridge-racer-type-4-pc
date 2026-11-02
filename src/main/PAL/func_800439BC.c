extern void func_8006E030();
extern void func_8006E09C();
extern void func_8006E108();
extern void func_80090CA4();
extern void func_80090DB4();
extern void func_80090EC4();

typedef struct { int a, b, c, d; } V;

void func_800439BC(char *a, int b, int *c) {
    int m1[8];
    int m2[10];
    int t[3];
    func_8006E09C(m1, 0x800 - *(short *)(a + 0x82));
    func_8006E108(m2, *(short *)(a + 0x80));
    func_80090CA4(m1, m2);
    func_8006E030(m2, *(short *)(a + 0x84));
    func_80090DB4(m1, m2);
    func_80090EC4(m2, b, t);
    *(V *)c = *(V *)(a + 0x10);
    c[0] = c[0] + t[0];
    c[1] = c[1] + t[1];
    c[2] = c[2] + t[2];
}
