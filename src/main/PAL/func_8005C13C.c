extern int func_80096434();
extern int func_80053E2C();
extern int func_80083C74();
extern void func_8007F910(int);
extern int func_8008A674(t);
extern int func_80053A80();
extern char D_800ACA30[];
extern int D_800F7620;
struct V4 { int a[4]; };
extern struct V4 D_80011260;
extern struct { char pad[0x24]; short s[4]; char pad2[0x2864 - 0x2C]; int i; } D_800F4DB8;

void func_8005C13C(int a0) {
    struct V4 v;
    int i, t;
    char *p = D_800ACA30;
    v = D_80011260;
    func_80096434(p, 0x64);
    p -= 0x3E0;
    *(int *)(p + 0x400) = 0xCE;
    *(int *)(p + 0x42C) = a0;
    *(int *)(p + 0x3E8) = 0x100;
    *(int *)(p + 0xB0) = 0;
    func_80053E2C();
    i = 0;
    if (D_800F4DB8.i >= 0) {
        func_80083C74(*(short *)&D_800F4DB8.i);
        func_8007F910(*(short *)&D_800F4DB8.i);
        D_800F4DB8.i = -1;
    }
    for (i = 0; i < 4; i++) {
        t = D_800F4DB8.s[v.a[i]];
        if (t >= 0) {
            func_8008A674(t);
            D_800F4DB8.s[v.a[i]] = -1;
        }
    }
    func_80053A80();
    D_800F7620 = -1;
}
