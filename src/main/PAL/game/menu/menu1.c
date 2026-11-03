#include "types.h"
#include "m2c_macros.h"
#include "game/menu/menu8.h"
#include "game/menu/menutran.h"

typedef struct { char p[0x17]; signed char b; char q[0x2C - 0x18]; } T;

typedef struct { char p0[0xC]; unsigned char a; char p1[7]; unsigned char b; char p2[0xF]; } E;

extern int func_80092CB8();
extern int func_8001ECE8();
extern int func_8002A604();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern int func_8002AF8C();
extern int D_800ABEB0;
extern short D_800AC5D0;
extern int D_800AC5DC;
extern short D_800AC64A;
extern signed char D_800AD4D8[];
extern int D_800F2DE4;
extern T D_800F2E30[];
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int func_8002A344();
extern int func_8003EC88();
extern E *D_800F4D80;
extern short D_800FFC18;
extern int D_800ACC14;
extern struct { int p[10]; int a28; int a2c; } D_800F3A38;
extern int func_8002B330();
extern int func_8004F24C();
extern int func_800207D4();
extern int func_8001FDD4();
extern int func_80031E3C();
extern int func_8003A198();
extern int func_8003A1B4();
extern int func_80041A18();
extern int func_80041A54();
extern int func_80096434();
extern int D_800ACC18;
extern short D_800AD4F2;
extern int *D_800F3A28;
extern short D_800F4CB0;
extern int D_800F4D40;
extern char D_800FFC48[];

void func_8002FCEC();
void func_8002FE98(void);
void func_8002FECC(void);
void func_8002FEFC();
void func_8003016C(void);

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

void func_8002FE98(void) {
    func_8002FEFC();
    func_8002FECC();
    func_8002A344(0x10086);
}

void func_8002FECC(void) {
    func_8003EC88(0);
    func_8003EC88(1);
    func_8003EC88(2);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)
#define D_800AC5D0 (*(unsigned short (*))&D_800AC5D0)
#define D_800F3A26 (*(unsigned short (*))&D_800F3A26)
#define D_800F4C6A (*(unsigned short (*))&D_800F4C6A)

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

#undef D_800F4D8A
#undef D_800AC5D0
#undef D_800F3A26
#undef D_800F4C6A

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 ((char *)D_800ACC14)

void func_8003016C(void)
{
    int *sp = (int *)0x1F800000;
    int *p;
    func_80041A18(D_800FFC48, 1);
    func_80041A54(0, 0, 3, 0xF0, 0xA0);
    func_80096434(0x1F800008, 0x20);
    p = D_800F3A28;
    p[7] = 0x60;
    p[8] = -0x110;
    p[9] = -0x160;
    p[5] = 0x1A;
    sp[6] = 0x400;
    func_8002F688();
    D_800F2DE4 = 0;
    D_800ABEB0 = 0;
    D_800AC5DC = 0x80;
    func_8003A198();
    if (D_800AD4F2 == 6) {
        func_80031E3C();
        func_8002A604();
        func_8003A1B4();
        D_800ACC18 = D_800F4D40;
    }
    D_800F4CB0 = 0;
    sp[0] = func_8001FDD4(D_800ACC14 + 0x70, sp[0], 0, 0, 0x140, 0x200, 8, 8, 8);
    D_800F4C6A = 1;
    D_800F3A26 = 0x10;
    D_800F4D8A = 0;
}

#undef D_800ACC14
