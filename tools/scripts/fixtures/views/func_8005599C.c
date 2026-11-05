#include "rr/matrix_words.h"

#define addPrim(ot, p) \
    ((DrTPage *)(p))->addr = (ot)->addr; \
    (ot)->addr = (u32)(p)

#define P D_800AC650.p

#define B(o) (*(unsigned char *)(P + (o)))

#define H(o) (*(short *)(P + (o)))

#define W(o) (*(int *)(P + (o)))

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(struct { char pad[0xDC]; s32 a; char pad2[4]; u8 b, c, d; } (*))&D_800AC650)

#undef D_800AC650

/* legacy typing of shared globals in this part of the unit */
#define func_80093BFC ((int (*)())func_80093BFC)

#define D_800ACC14 ((S *)D_800ACC14)

#undef func_80093BFC

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(struct { char pad[0xD4]; s32 a, b, c, d; u8 e[4]; } (*))&D_800AC650)

#undef D_800AC650

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(struct { char pad[0x3DC]; s32 a; } (*))&D_800AC650)

#undef D_800AC650

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(struct { char pad[0xEC]; char buf[0x160]; char *p; } (*))&D_800AC650)

#undef D_800AC650

typedef struct { int w[4]; } Q16;

typedef RrMatrixWords32 M32;

typedef struct { short m[3][3]; short pad[7]; } S70;

extern Q16 D_800111D4;

extern short D_800ABED0[][3];

extern int func_8006E030();

extern int func_8006E09C();

extern int func_8006E108();

extern int MulMatrix0();

extern int MulMatrix();

extern int MulMatrix2();

extern int func_80090F74();

extern int SetLightMatrix();

extern int SetColorMatrix();

extern int SetBackColor();

void func_8005599C(a0, a1, a2, a3) char *a0; char *a1; M32 *a2; M32 *a3; {
    M32 s10, s30, s50;
    S70 s70;
    Q16 s90;
    int n;
    s90 = D_800111D4;
    func_8006E09C(&s50, -*(short *)(a0 + 0x12));
    MulMatrix2(0x1F800028, &s50);
    func_8006E108(&s10, -*(short *)(a0 + 0x10));
    func_8006E030(&s30, -*(short *)(a0 + 0x14));
    MulMatrix(&s10, &s30);
    MulMatrix2(&s50, &s10);
    func_8006E09C(&s50, *(short *)(a0 + 0x12));
    func_8006E108(&s30, *(short *)(a0 + 0x10));
    MulMatrix(&s50, &s30);
    func_8006E030(&s30, *(short *)(a0 + 0x14));
    MulMatrix2(&s50, &s30);
    *a3 = s30;
    n = 0;
    do {
        D_800ABED0[n][0] = *(unsigned short *)(a1 + n * 8 + 0xC);
        D_800ABED0[n][1] = *(unsigned short *)(a1 + n * 8 + 0xE);
        D_800ABED0[n][2] = *(unsigned short *)(a1 + n * 8 + 0x10);
        s70.m[0][n] = (unsigned char)a1[n * 4] * 16;
        s70.m[1][n] = (unsigned char)a1[n * 4 + 1] * 16;
        s70.m[2][n] = (unsigned char)a1[n * 4 + 2] * 16;
        n++;
    } while (n < 3);
    MulMatrix0(D_800ABED0, &s10, &s30);
    SetLightMatrix(&s30);
    SetColorMatrix(&s70);
    SetBackColor((unsigned char)a1[0x24], (unsigned char)a1[0x25], (unsigned char)a1[0x26]);
    func_80090F74(&s10, &s90);
    *a2 = s10;
}
