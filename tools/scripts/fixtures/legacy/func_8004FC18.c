extern void func_8004FB00();
extern void func_80052B8C(int, short);
extern struct { char pad[0x18]; unsigned short a; short b; short c; unsigned short d; unsigned short e; } D_800F4DB8;

void func_8004FC18(short arg0, short arg1) {
    if (arg1 >= 2) {
        D_800F4DB8.b = arg1;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = arg0;
        D_800F4DB8.e = D_800F4DB8.a;
    } else {
        D_800F4DB8.a = arg0;
        D_800F4DB8.b = 0;
        D_800F4DB8.c = 0;
        D_800F4DB8.d = 0;
        D_800F4DB8.e = 0;
        if (arg0 == 0) {
            func_8004FB00();
        }
    }
    func_80052B8C(arg0 & 0xFFFF, arg1);
}
