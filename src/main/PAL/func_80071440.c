extern int func_80092CB8();
extern int func_8001ECE8();
extern int D_800AC5DC;
extern int D_8010E7A4;
struct S { unsigned char c[8]; };
extern struct S D_80011324, D_8001132C, D_80011334, D_8001133C, D_80011344;

void func_80071440(void) {
    struct S a, b, c, d, e, f;
    a = D_80011324;
    b = D_80011324;
    c = D_8001132C;
    d = D_80011334;
    e = D_8001133C;
    f = D_80011344;
    func_80092CB8(0);
    func_8001ECE8(0, 0, 0, 3);
    D_800AC5DC = 0x80;
    D_8010E7A4 = 0;
}
