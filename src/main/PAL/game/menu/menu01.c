#include "types.h"
#include "m2c_macros.h"

extern unsigned char D_8009F7E4[];
extern int func_8002B330();
extern int func_8001E378();
extern int func_8002A344();
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

void func_800397A4(unsigned char *a0, int a1);
void func_800397BC(unsigned char *a0);
void func_8003984C(void);
void func_80039894(void);
void func_8003989C(void);
void func_80039920(void);
void func_80039988(void);

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
