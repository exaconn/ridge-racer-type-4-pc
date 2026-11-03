#include "types.h"
#include "m2c_macros.h"

typedef struct { char b[0x17]; signed char b17; char r[0x2C - 0x18]; } w3g2S;

extern int func_800207D4(), func_8002A344(), func_8002AF8C(), func_8002B330(), func_8002F688(), func_80032A14(), func_8003A198();
extern int func_8003B398(), func_8003D6F0(), func_8004F24C();
extern struct { char pad[4]; char b4; char pad5; signed char b6; char pad2[0xC - 7]; } *D_800AC590;
extern short D_800AC5D0, D_800F3A26, D_800F4BE8, D_800F4C6A, D_800F4D3C;
extern int D_800ACC14, D_800ACC18, D_800F4D74;
extern w3g2S D_800AD4D8, D_800F2E30[];
extern struct { char pad[0x28]; int w28, w2C; } D_800F3A38;
extern struct { char pad[8]; short s8, sA; char padC[4]; short s10; char pad12[0x2C - 0x12]; short s2C; } *D_800F3AFC;
extern char *D_800F4D80;
extern unsigned short D_800F4D8A;
extern unsigned short D_800F3A40, D_800F3A42;
extern int D_8010BFC0;
extern int func_8002A344();
extern int func_8003D228();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern s16 D_800AC64A;
extern s32 D_800F2DE4;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800FFC00;
extern s16 D_800FFC18;

void func_80031FE8(void);
void func_8003244C(void);
void func_800324F8(void);

void func_80031FE8(void) {
    int s0;
    switch (D_800F4D8A) {
    case 0:
        *(D_800F4D80 + D_800F4D3C * 0x24 + 0xC) = 0;
        func_8002B330(0, 2, &D_800F4D3C, 3, 0);
        *(D_800F4D80 + D_800F4D3C * 0x24 + 0xC) = 2;
        if (D_800F3AFC->s10 == 3) {
            s0 = D_800AC590->b6;
            if (s0 == 4) {
                func_8002AF8C(1, 1, 1);
                if (D_800F3A38.w28 != 0) {
                    if (D_800F2E30[D_800F4D3C].b17 > 0) {
                        func_8004F24C(1);
                        D_800F4D8A = 1;
                        D_800AC5D0 = 0x40;
                        func_8003A198();
                        D_800ACC18 = D_800F4D74;
                        D_800F3AFC->s10 = s0;
                        D_800F3AFC->s2C = s0;
                    } else {
                        func_8004F24C(4);
                    }
                } else if (D_800F3A38.w2C != 0) {
                    func_8004F24C(2);
                    D_800F4D8A = 2;
                    D_800AC5D0 = 0x40;
                    func_8003A198();
                    D_800ACC18 = D_800F4D74;
                    func_8003B398(0);
                    D_800F3AFC->s10 = s0;
                    D_800F3AFC->s2C = s0;
                }
            }
        }
        goto common;
    case 1:
        func_8002AF8C(0, 1, 1);
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            if (func_8003B398(1) == 1) {
                D_800AD4D8 = D_800F2E30[D_800F4D3C];
                D_800F4C6A = 1;
                D_800F3A26 = 0xF;
                D_800F4D8A = 0;
                return;
            }
        }
        goto common;
    case 2:
        func_8002AF8C(0, 1, 1);
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            func_8002F688();
            D_800F4BE8 = 1;
            D_800F4C6A = 1;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
            return;
        }
    default:
    common:
        func_8003D6F0(0);
        func_8003D6F0(1);
        if (D_800F3AFC->s10 == 3) {
            D_800AC590->b4 = 0x1C;
            func_80032A14(D_800F3AFC->s8 + 0xA, D_800F3AFC->sA + 0x28, D_800AC590);
        }
        func_8002A344(0x10086);
    }
}

void func_8003244C(void) {
    if (D_800F3A42 & 0x1000) {
        D_8010BFC0 = D_8010BFC0 - 1;
    } else if (D_800F3A42 & 0x4000) {
        D_8010BFC0 = D_8010BFC0 + 1;
    }
    if (D_8010BFC0 < 0) {
        D_8010BFC0 = 1;
    } else if (D_8010BFC0 >= 2) {
        D_8010BFC0 = 0;
    }
    if (D_800F3A40 & 0x800) {
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
        D_800F4D8A = 0;
    }
}

void func_800324F8(void) {
    D_800F2DE4 = 0;
    D_800FFC00 = 0;
    D_800FFC18 = 3;
    D_800AC64A = 0;
    func_8003E928(0, 0x8C, 0x88, 0x1C, 0x20, 0, 0, 1, 0, 0x68);
    func_8003E928(1, 0x8C, 0xB0, 0x1C, 0x20, 0, 0, 1, 0, 0x69);
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    D_800F4C6A = 1;
    D_800F3A26 = 0x2B;
    D_800F4D8A = 0;
    M2C_FIELD(D_800F3AFC, s16 *, 2) = 1;
    func_8002A344(0x10086);
}
