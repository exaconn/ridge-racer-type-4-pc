extern int func_80054D30();
extern int func_8004F274();
extern struct {
    char pad0[0x3E4]; int f3E4; char pad1[4]; int f3EC; char pad2[4]; int f3F4; int f3F8; char pad3[4];
    int f400; int f404; int f408; char pad4[4]; short f410, f412, f414, f416, f418, f41A; int f41C; char pad5[4];
    int f424; char pad6[4]; int f42C; int f430; int f434; char pad7[8]; int f440;
} D_800AC650;

void func_8005C7A4(void) {
    int v, w, x;
    D_800AC650.f434 = 0;
    D_800AC650.f440 = 0;
    v = D_800AC650.f408;
    if (v == 1 || (v == 0 && D_800AC650.f3EC == 0)) {
        if (D_800AC650.f3F4 == 0) {
            func_80054D30(1, 0, 0x100);
            D_800AC650.f3F4 = 1;
        }
    }
    w = D_800AC650.f408;
    if (w == 2 || (w == 0 && D_800AC650.f3EC == 0xFF)) {
        if (D_800AC650.f3F4) {
            func_80054D30(0, 0, 0);
            D_800AC650.f3F4 = 0;
        }
    }
    D_800AC650.f400 = 0x8C;
    D_800AC650.f404 = 0x100;
    D_800AC650.f3F8 = 0;
    D_800AC650.f410 = 1;
    D_800AC650.f412 = 0x10;
    D_800AC650.f41A = 0x10;
    D_800AC650.f416 = 0x10;
    D_800AC650.f418 = 1;
    D_800AC650.f414 = 1;
    D_800AC650.f3E4 = 0xBB8;
    x = D_800AC650.f430;
    D_800AC650.f424 = 0;
    D_800AC650.f41C = 1;
    if (x == 0) {
        func_8004F274(D_800AC650.f42C);
        D_800AC650.f430 = 1;
    }
}
