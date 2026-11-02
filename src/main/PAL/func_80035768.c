extern int func_8001ECE8();
extern int func_8002A604();
extern int func_80092CB8();
extern int func_8009AEAC();
extern char D_800ABEBC[];
extern int D_800AC5A4;
extern int D_800AC5DC;
extern int D_800F2DE4;
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern short D_800FF688;

void func_80035768(void) {
    unsigned a;
    func_80092CB8(0);
    func_8001ECE8(0, 0, 0, 2);
    D_800ABEBC[0] = 1;
    D_800ABEBC[1] = 0;
    D_800ABEBC[2] = 1;
    D_800ABEBC[3] = 0;
    func_8002A604();
    D_800AC5A4 = 0x1F4;
    D_800F2DE4 = 0;
    D_800AC5DC = 0x80;
    D_800F4D8A = 0;
    D_800FF688 = 0;
    a = ((unsigned)func_8009AEAC(0, 0, 0) >> 7) & 3;
    if (a == 3) {
        D_800F4C6A = 1;
        D_800F3A26 = a;
        D_800F4D8A = 0;
    } else {
        D_800F4C6A = 1;
        D_800F3A26 = 0x1E;
        D_800F4D8A = 0;
    }
}
