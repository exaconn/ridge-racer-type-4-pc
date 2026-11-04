#include "types.h"
#include "scratchpad.h"
#include "m2c_macros.h"
#include "game/system/frmtask.h"
#include "game/gfx/meshquad.h"
#include "game/menu/menuwork.h"
#include "rr/menu_background.h"

#define P8(base, off) (*(unsigned char *)((char *)(base) + (off)))
#define IDX30(base, i) (*(unsigned char *)((char *)(base) + (i) * 0x24 + 0x30))

typedef struct { int x; } __attribute__((packed)) W7C0_U;

typedef struct { char pad[0x24]; } W7C0_E;

typedef struct { char p0; signed char b1; unsigned char b2; } W7C0_S;

typedef struct { char pad[2]; short s2; char pad2[0xC]; short s10; } W7C0_F;

extern unsigned char D_8009F7E4[];
extern int func_8002B330();
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern unsigned char D_8009F810[];
extern unsigned char D_800AD4D8[];
extern unsigned char D_800AD4EB;
extern unsigned char D_800F3920[];
extern int func_80013890();
extern int func_8002E4F4();
extern int func_80031E3C();
extern int func_80033A74();
extern int func_80037110();
extern int func_8003A198();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern s32 D_800ABEB0;
extern M2C_UNK D_800ABEF8;
extern s32 D_800AC5DC;
extern s16 D_800AD50A;
extern int func_8002AF8C();
extern int func_8002CC88();
extern int func_8002E5A0();
extern int func_8002E828();
extern int func_8003FBA8();
extern int func_80052B8C();
extern int func_80053BB4();
extern int func_8006E1AC();
extern int func_8006EF8C();
extern unsigned char D_8009F818[];
extern short D_800AC5D0;
extern signed char D_800AD4EF;
extern signed char *D_800F2AB0;
extern int D_800ACC18;
extern int D_800F3A60;
extern W7C0_F *D_800F3AFC;
extern int D_800F4D70;
extern W7C0_E *D_800F4D80;
extern unsigned char D_800F4D98[];
extern void func_80037198();
extern int func_8003AE4C();
extern void func_8003D6F0();
extern void func_8003EC88();
extern void func_8004F24C();
extern void func_8005AF5C();

void func_800397A4(unsigned char *a0, int a1);
void func_800397BC(unsigned char *a0);
void func_8003984C(void);
void func_80039894(void);
void func_8003989C(void);
void func_80039920(void);
void func_80039ABC(void);
void func_80039C0C(void);

void func_800397A4(unsigned char *a0, int a1) {
    *a0 = D_8009F7E4[a1];
}

void func_800397BC(unsigned char *a0) {
    int i;
    for (i = 0; i < 0x29; i++) {
        if (*a0 == D_8009F7E4[i]) break;
    }
    func_8002B330(2, 4, &i, 0x29, 0);
    *a0 = D_8009F7E4[i];
}

void func_8003984C(void) {
    D_800F2DE4 = 0;
    func_8001E378();
    func_8002A344(0x1000C);
    D_800F4C6A = 0;
    D_800F3A26 = 6;
    D_800F4D8A = 0;
}

void func_80039894(void) {
}

void func_8003989C(void) {
    int v;
    if (D_800AD4D8[0x1E] == 0x12) {
        int i = D_800AD4D8[0x1C] * 2 + (D_800AD4D8[0x1D] >> 1);
        v = D_8009F810[i];
    } else {
        v = 1;
    }
    if (v < 0) {
        v = 0;
    } else if (v >= 9) {
        v = 8;
    }
    D_800AD4D8[0x12] = v;
    if (v == 1) {
        D_800AD4D8[0x13] = D_800AD4D8[0x18];
    }
}

void func_80039920(void) {
    int i = D_800AD4D8[0x1C] * 2 + (D_800AD4D8[0x1D] >> 1);
    int v = D_8009F810[i];
    if (v < 0) {
        v = 0;
    } else if (v >= 9) {
        v = 8;
    }
    D_800AD4EB = D_800F3920[v + 0x60];
}

void func_80039988(void) {
    func_80013890(5);
    func_8003A198();
    func_80033A74(1);
    func_8002E4F4(&D_800ABEF8);
    M2C_FIELD(&D_800ABEF8, s32 *, 0x28) = 0x134;
    M2C_FIELD(&D_800ABEF8, s32 *, 0x24) = 0xA;
    M2C_FIELD(&D_800ABEF8, s32 *, 0x54) = 0xA00;
    func_80037110();
    func_8003E928(0, 0x18, 0x58, 0x44, 0x20, 0, 0, 1, 0, 0x53);
    func_8003E928(1, 0x74, 0x58, 0x1C, 0x20, 5, 0, 1, 0, 0x37);
    func_8003E928(2, 0x74, 0x80, 0x1C, 0x20, 5, 0, 1, 0, 0x38);
    D_800ABEB0 = 0;
    D_800F2DE4 = 0;
    D_800AD50A = 0x122;
    D_800AC5DC = 0x80;
    func_80031E3C();
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(char (*)[])&D_800ABEF8)
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

void func_80039ABC(void) {
    D_800F2DE4 += 1;
    func_8006EF8C(6);
    if (D_800F4D8A == 0) {
        if (func_800578D8(D_8009F818[D_800AD4EF / 2]) != 0) {
            D_800AC5D0 = 0x40;
            func_80052B8C(0, 0x3C);
            D_800F4D8A = 1;
            func_80039988();
            D_800F2AB0[1] = 0;
        }
    } else {
        func_8006E1AC();
        func_8002E828(2, (SCRATCHPAD + 0x28));
        func_8002E5A0(D_800ABEF8);
        func_8002CC88(D_800ABEF8, 0, 0x100, 1);
        func_8003FBA8(0);
        if (D_800F2AB0[1] == 2) {
            D_800F4C6A = 1;
            D_800F3A26 = 0x22;
            D_800F4D8A = 0;
            func_80053BB4(0);
        }
    }
    func_8002A344(0x10028);
    func_8002AF8C(0, 0, 0);
}

#undef D_800ABEF8
#undef D_800F4D8A

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(char (*)[])&D_800ABEF8)
#define D_800AD4D8 (*(char (*)[])&D_800AD4D8)
#define D_800F2AB0 ((W7C0_S *)D_800F2AB0)
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)
#define func_8002A344 ((void (*)())func_8002A344)
#define func_8002AF8C ((void (*)())func_8002AF8C)
#define func_8002B330 ((void (*)())func_8002B330)
#define func_8002CC88 ((void (*)())func_8002CC88)
#define func_8002E5A0 ((void (*)())func_8002E5A0)
#define func_80033A74 ((void (*)())func_80033A74)
#define func_8003A198 ((void (*)())func_8003A198)
#define func_8003FBA8 ((void (*)())func_8003FBA8)
#define func_8006EF8C ((void (*)())func_8006EF8C)

void func_80039C0C(void)
{
    int s0, s2, c2, v1, a0, a1;
    char *t0;
    unsigned char *q;
    unsigned char sp18[4];
    int s1;

    D_800F2DE4 = D_800F2DE4 + 1;
    func_8002A344(0x10022);
    s2 = func_8003AE4C(1, 3);
    if (s2 == 1) {
        v1 = D_800ACC18;
        s0 = D_800F4D8A;
        D_800F4D70 = v1;
        if (s0 == s2)
            goto b1;
        if (s0 < 2) {
            if (s0 == 0)
                goto b0;
            goto end;
        }
        if (s0 == 2)
            goto b2;
        goto end;
    b0:
        s0 = P8(D_800F4D80, 0x14);
        if (s0 == 2) {
            func_8002AF8C(1, 1, 0);
            P8(D_800F4D80, 0xC) = s0;
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                P8(D_800F4D80, 0xC) = s2;
                P8(D_800F4D80, 0x38) = 0;
                P8(D_800F4D80, 0x5C) = 0;
                D_800F4D8A = s2;
            }
        }
        goto end;
    b1:
        t0 = (char *)D_800F4D80;
        v1 = P8(t0, 0x38);
        c2 = 2;
        if (v1 == c2) {
            q = D_800F4D98;
            IDX30(t0, q[3]) = 0;
            func_8002B330(0, 1, q + 3, c2, 0);
            IDX30(D_800F4D80, q[3]) = c2;
            if (D_800F3A60 != 0) {
                func_8004F24C(1);
                a1 = 0;
                a0 = 0;
                IDX30(D_800F4D80, q[3]) = s2;
                for (a1 = 0; a1 < 3; a1++)
                    P8(D_800F4D80, a1 * 0x24 + 0x14) = 3;
            }
        } else if (v1 == 5) {
            D_800F4D8A = c2;
        }
        goto end;
    b2:
        D_800F2DE4 = 0;
        D_800F2AB0->b1 = 3;
        D_800F3AFC->s10 = 4;
        func_8003A198();
        {
        char *b = D_800AD4D8;
        *(W7C0_U *)(b + 0x1C) = *(W7C0_U *)D_800F4D98;
        if ((signed char)b[0x17] <= 0)
            *(short *)(b + 0x1A) = s0;
        }
        func_8005AF5C();
        D_800F4C6A = s2;
        D_800F3A26 = 0xB;
        D_800F4D8A = 0;
    }
end:
    func_80033A74(1);
    func_8006EF8C(6);
    if (D_800F2AB0->b1 > 0) {
        char *g = D_800ABEF8;
        sp18[0] = 0x80;
        sp18[1] = 0x80;
        sp18[2] = 0x80;
        *(int *)(g + 0x54) = (*(int *)(g + 0x54) + 8) & 0xFFF;
        if (D_800F2AB0->b1 == 2)
            s1 = 0x100;
        else
            s1 = (D_800F2AB0->b2 << 8) / 30;
        D_800F3AFC->s2 = 1;
        func_8003EC88(0);
        func_8003EC88(1);
        func_8003EC88(2);
        func_80037198(0);
        func_8002E5A0(D_800ABEF8);
        func_8002CC88(D_800ABEF8, *(int *)sp18, s1, 1);
    }
    func_8003D6F0(0);
    func_8003FBA8(0);
}

#undef D_800ABEF8
#undef D_800AD4D8
#undef D_800F2AB0
#undef D_800F4D8A
#undef func_8002A344
#undef func_8002AF8C
#undef func_8002B330
#undef func_8002CC88
#undef func_8002E5A0
#undef func_80033A74
#undef func_8003A198
#undef func_8003FBA8
#undef func_8006EF8C
