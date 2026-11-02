extern void func_80096434();
extern struct {
    char pad0[0x18C0]; short a; short b; char pad1[0x28];
    int c; char pad2[0x18]; int d; char pad3[0x18]; int e; short f; char pad4[0x16]; int g; short h;
} D_800F4DB8;
extern int D_800F66A0;

void func_80051B44(short arg0, short arg1) {
    int t;
    D_800F4DB8.a = arg0;
    D_800F4DB8.b = arg1;
    func_80096434((char *)&D_800F4DB8 + 0x18EC, 0x70);
    t = arg1 & 0xFFFF;
    switch (t) {
    case 0:
        D_800F4DB8.c = 1;
        D_800F4DB8.d = 1;
        D_800F4DB8.e = 0;
        D_800F4DB8.f = 0;
        break;
    case 1:
        D_800F4DB8.e = 0;
        D_800F4DB8.g = 0;
        D_800F4DB8.f = 0;
        D_800F4DB8.h = t;
        break;
    }
    D_800F66A0 = 4;
}
