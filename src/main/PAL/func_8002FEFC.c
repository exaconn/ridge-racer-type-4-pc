typedef struct { char p0[0xC]; unsigned char a; char p1[7]; unsigned char b; char p2[0xF]; } E;
extern E *D_800F4D80;
extern short D_800AC64A;
extern unsigned short D_800F4D8A, D_800AC5D0, D_800F3A26, D_800F4C6A;
extern short D_800FFC18;
extern int D_800ACC14;
extern struct { int p[10]; int a28; int a2c; } D_800F3A38;
extern int func_8002B330();
extern int func_8002AF8C();
extern int func_8004F24C();
extern int func_800207D4();
extern int func_8002F688();

void func_8002FEFC() {
    int s;
    int v, t, w;
    s = D_800F4D8A;
    if (s != 0) goto C1;
    D_800F4D80[D_800AC64A].a = 0;
    func_8002B330(0, 2, &D_800AC64A, 3, 0);
    D_800F4D80[D_800AC64A].a = 2;
    s = D_800F4D80->b;
    if (s != 2) goto END;
    func_8002AF8C(1, 1, 1);
    if (D_800F3A38.a28 == 0) goto L0C;
    func_8004F24C(1);
    D_800F4D80[D_800AC64A].a = 1;
    v = 3;
    if (D_800AC64A < 2) v = 7;
    D_800F4D80->b = v;
    D_800F4D80[1].b = v;
    D_800F4D80[2].b = v;
    D_800AC5D0 = 0x40;
    D_800F4D8A = 1;
    goto END;
L0C:
    if (D_800F3A38.a2c == 0) goto END;
    func_8004F24C(2);
    v = 3;
    D_800AC64A = s;
    D_800F4D80->b = 3;
    D_800F4D80[1].b = v;
    D_800F4D80[2].b = v;
    D_800AC5D0 = 0x40;
    D_800F4D8A = 1;
    goto END;
C1:
    if (s != 1) goto END;
    func_8002AF8C(0, 1, 1);
    t = D_800AC5D0 - 1;
    D_800AC5D0 = t;
    func_800207D4(D_800ACC14 + 0x70, (0x40 - (short)t) * 4, 0x45);
    if ((short)D_800AC5D0 > 0) goto END;
    w = D_800AC64A;
    if (w == s) goto L120;
    if (w < 2) {
        if (w == 0) goto L104;
        goto END;
    }
    if (w == 2) goto L13C;
    goto END;
L104:
    func_8002F688();
    D_800F4C6A = 0;
    D_800F3A26 = s;
    D_800F4D8A = 0;
    goto END;
L120:
    D_800FFC18 = 4;
    D_800F4C6A = s;
    D_800F3A26 = 0xD;
    D_800F4D8A = 0;
    goto END;
L13C:
    D_800F4C6A = s;
    D_800F3A26 = 3;
    D_800F4D8A = 0;
END:
    return;
}
