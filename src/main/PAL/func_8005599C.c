typedef struct { int w[4]; } Q16;
#include "rr/matrix_words.h"
typedef RrMatrixWords32 M32;
typedef struct { short m[3][3]; short pad[7]; } S70;
extern Q16 D_800111D4;
extern short D_800ABED0[][3];
extern int func_8006E030();
extern int func_8006E09C();
extern int func_8006E108();
extern int func_80090914();
extern int func_80090CA4();
extern int func_80090DB4();
extern int func_80090F74();
extern int func_800910E4();
extern int func_80091114();
extern int func_80091274();

void func_8005599C(a0, a1, a2, a3) char *a0; char *a1; M32 *a2; M32 *a3; {
    M32 s10, s30, s50;
    S70 s70;
    Q16 s90;
    int n;
    s90 = D_800111D4;
    func_8006E09C(&s50, -*(short *)(a0 + 0x12));
    func_80090DB4(0x1F800028, &s50);
    func_8006E108(&s10, -*(short *)(a0 + 0x10));
    func_8006E030(&s30, -*(short *)(a0 + 0x14));
    func_80090CA4(&s10, &s30);
    func_80090DB4(&s50, &s10);
    func_8006E09C(&s50, *(short *)(a0 + 0x12));
    func_8006E108(&s30, *(short *)(a0 + 0x10));
    func_80090CA4(&s50, &s30);
    func_8006E030(&s30, *(short *)(a0 + 0x14));
    func_80090DB4(&s50, &s30);
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
    func_80090914(D_800ABED0, &s10, &s30);
    func_800910E4(&s30);
    func_80091114(&s70);
    func_80091274((unsigned char)a1[0x24], (unsigned char)a1[0x25], (unsigned char)a1[0x26]);
    func_80090F74(&s10, &s90);
    *a2 = s10;
}
