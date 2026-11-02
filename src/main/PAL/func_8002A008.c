extern int func_80020074();
extern int func_800207D4();
extern int func_80029E0C();
extern int func_8002E0D8();
extern int func_8003660C();
extern int func_80036FD4();
extern int func_80037D88();
extern int func_8004417C();
extern int func_8004B864();
extern int func_8004BB54();
extern int func_8004EE60();
extern int func_8005007C();
extern int func_80052A30();
extern int func_800555E0();
extern int func_800556B4();
extern int func_8006E1AC();
extern void func_8006ECE8(s32);
extern int func_8006EF8C();
extern int func_800735D8();
extern int func_800747EC();
extern int func_80090424();
extern int D_800ABEB0;
extern struct { char pad[0x184]; int f184; char pad2[0x320 - 0x188]; } D_800ABEF8[];
extern short D_800AC59C;
extern int D_800AC5A8;
extern short D_800AC5D0;
extern int D_800ACC14;
extern int D_800F2A64;
extern short D_800F2AB4[];
extern int D_800F2DE4;
extern short D_800F4C70[];
extern short D_800F4D44;
extern struct { char pad[0x1E4]; short f1e4; } *D_800FFC20[];
extern int D_8010BFB0;

void func_8002A008(void) {
    int i;
    short t;
    struct { char pad[0x1E4]; short f1e4; } **pp;
    D_800F2DE4 += 1;
    if (D_800AC5A8 < 0x1A) {
        func_800207D4(D_800ACC14 + 0x70, (0x19 - D_800AC5A8) * 10, 0x25);
        if (D_800AC5A8 == 10) {
            func_80052A30(0);
        }
    } else {
        func_800556B4();
    }
    D_800ABEB0 += 1;
    if (D_800AC5D0 >= 0) {
        if (D_800AC5D0 == 0) {
            func_8004EE60(0);
            func_8003660C();
        }
        t = (unsigned short)D_800AC5D0 - 1;
        D_800AC5D0 = t;
        if (t < 0x19) {
            func_800207D4(D_800ACC14 + 0x70, (0x19 - t) * 10, 0x45);
        }
    }
    if (func_80090424((D_800AC5A8 << 7) & 0xF80) > 0) {
        { int tt = *(int *)0x1F800000; *(int *)0x1F800000 = func_80020074(D_800ACC14 + 0x70, tt, 0x10, 0x10, 0x4C, 0xC, 0x64, 0xC0, 5, 0x7FF8, 0, 0x80); }
    }
    for (i = 0; i < D_800AC59C; i++) {
        if (D_800FFC20[i]->f1e4 >= 0x10) {
            func_80029E0C(D_800FFC20[i]);
        }
    }
    func_80037D88(D_800F4D44);
    if ((unsigned)((unsigned short)D_800F4D44 - 2) < 2U) {
        if ((unsigned char)D_800AC5A8 == 0) {
            if (D_800ABEF8[0].f184 < D_800ABEF8[1].f184) {
                D_8010BFB0 = 1;
            } else {
                D_8010BFB0 = 0;
            }
        }
    }
    func_8006EF8C(0);
    func_8004417C(2, &D_800ABEF8[D_8010BFB0]);
    func_800735D8(0);
    func_8006E1AC();
    func_8002E0D8(6);
    *(int *)0x1F800078 = D_800F2A64;
    func_8006ECE8(0);
    func_800747EC(0);
    func_80036FD4(0);
    if (D_800AC5A8 >= 0x19) {
        func_800555E0();
    }
    func_8005007C(D_800F4C70[0], D_800F2AB4[0], 0);
    func_8005007C(D_800F4C70[1], D_800F2AB4[1], 1);
    func_8004B864(0);
    func_8004B864(1);
    func_8004BB54(0);
    func_8004BB54(1);
}
