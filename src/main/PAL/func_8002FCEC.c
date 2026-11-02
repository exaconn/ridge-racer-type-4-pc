extern int func_80092CB8();
extern int func_8001ECE8();
extern int func_80031DB0();
extern int func_8002A604();
extern int func_8002F688();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern int func_8002AF8C();
extern int D_800ABEB0;
extern short D_800AC5D0;
extern int D_800AC5DC;
extern short D_800AC64A;
extern signed char D_800AD4D8[];
extern int D_800F2DE4;
typedef struct { char p[0x17]; signed char b; char q[0x2C - 0x18]; } T;
extern T D_800F2E30[];
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;

void func_8002FCEC() {
    int a0, a1;
    short v;
    T *p;
    func_80092CB8(0);
    func_8001ECE8(0, 0, 0, 3);
    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    D_800AC5D0 = 0;
    D_800AC64A = 1;
    D_800AC5DC = 0x80;
    D_800AD4D8[0x17] = -1;
    D_800AD4D8[0x19] = 9;
    func_80031DB0();
    func_8002A604();
    a1 = 0;
    a0 = 0;
    p = D_800F2E30;
    L:
    a0++;
    if (p->b > 0) goto T;
    p++;
    if (a0 < 3) goto L;
    J:
    if (a1 != 0) goto B;
    func_8002F688();
    D_800F4C6A = 0;
    D_800F3A26 = 1;
    D_800F4D8A = 0;
    goto E2;
    T:
    a1 = 1;
    goto J;
    B:
    func_8003E928(0, 0x6C, 0xB8, 0x44, 0x20, 0, 0, 1, 0, 0x54);
    func_8003E928(1, 0x6C, 0xE0, 0x44, 0x20, 0, 0, 1, 0, 0x67);
    func_8003E928(2, 0x6C, 0x108, 0x44, 0x20, 0, 0, 1, 0, 0x55);
    func_8002AF8C(0, 1, 1);
    D_800F4C6A = 1;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
    E2:
}
