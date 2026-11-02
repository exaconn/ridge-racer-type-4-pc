/* RODATA 0x80011118 0xc */
extern int func_8004EE08();
extern int func_8004F2A0();
extern int func_8004FBF0();
extern int func_80050E58();
extern int func_80051248();
extern int func_800514FC();
extern int func_800517D8();
extern int func_80051B28();
extern int func_80051B44();
extern void func_8007F910(int);
extern int func_80083C74();
extern int func_80084534();
extern int func_8008A674();
extern struct {
    char pad00[0x24];
    short h24[0x1A];
    char pad0[0x1A08-0x24-0x34];
    int f1a08;
    int a1a0c[7];
    char pad2[0x1A2C - 0x1A28];
    int f1a2c;
    int f1a30;
    char pad3[0x2864 - 0x1A34];
    int f2864;
    int f2868;
} D_800F4DB8;
void func_8004F82C(int a0, int a1) {
    int a[3] = { 2, 3, 7 };
    int i;
    int *p;
    char *g;
    func_80084534(0);
    if (D_800F4DB8.f2864 >= 0) {
        func_80083C74((short)D_800F4DB8.f2864);
        func_8007F910((short)D_800F4DB8.f2864);
        D_800F4DB8.f2864 = -1;
    }
    for (i = 0; i < 3; i++) {
        if (D_800F4DB8.h24[a[i]] >= 0) {
            func_8008A674(D_800F4DB8.h24[a[i]]);
            D_800F4DB8.h24[a[i]] = -1;
        }
    }
    func_8004FBF0();
    func_80050E58();
    func_80051248(a0);
    func_800514FC();
    func_8004F2A0();
    func_800517D8();
    func_8004EE08();
    if (a0 == 0) {
        func_80051B44(a1 & 0xFFFF, 0);
    } else {
        func_80051B44(a1 & 0xFFFF, 1);
    }
    func_80051B28();
    func_80084534(0);
    D_800F4DB8.f1a08 = 0;
    i = 6;
    do {
        D_800F4DB8.a1a0c[i] = 1;
        i--;
    } while (i >= 0);
    D_800F4DB8.f1a30 = 0;
    D_800F4DB8.f1a2c = 0;
    D_800F4DB8.f2868 = -1;
}
