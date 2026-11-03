#include "types.h"
#include "m2c_macros.h"

extern unsigned char D_800A4478[];
extern char *D_800AC590;
extern short *D_800F3AFC;
extern short D_800F4D3C, D_800FFC00, D_800FFC18, D_80109490, D_801094A0;
extern unsigned char D_8010B2DC[];
extern int func_8003EC88();
extern char *D_800F4D80;
extern short D_800FFC18;
extern int func_8001F400();
extern int func_8001FD20();
extern char D_80011680[];
extern char D_80011688[];
extern char D_80011690[];
extern int D_800ACC14;

void func_8007D1B8();
void func_8007D448(int a0, int a1);
void func_8007D558(void);
void func_8007D560(int a0, int a1);
void func_8007D65C(int a0);
void func_8007D710(void);

void func_8007D1B8(a0, a1, a2, a3, a4, a5) int a0; int a1; int a2; int a3; int a4; int a5; {
    unsigned char *v1;
    int s0 = a1;
    if (a2 != 0) {
        int c14 = 0x14, c16 = 0x16, c4 = 4, c1 = 1;
        v1 = D_800A4478;
        a3 = (int)v1 + c14;
L1:
        if (a5 != v1[1]) goto next;
        if (a5 == c14) goto done;
        if (a0 != c16 || s0 != c4 || a2 != c1) s0 = 0;
        a0 = v1[0];
        goto done;
next:
        v1 += 4;
        if ((int)v1 < a3) goto L1;
done:;
        if (a0 == 5) {
            if (D_801094A0 == 1) {
                a0 = 0x1A;
            } else if (D_800FFC18 == 2 && D_800FFC00 == 1) {
                a0 = 0x21;
            }
        }
    }
    if (D_800F3AFC[8] == 3) {
        if (a0 == -1) {
            if (D_800FFC18 == 3 || D_800FFC18 == 6 || D_800FFC18 == 7) a0 = 0x19;
        } else if (a0 == -128) {
            a0 = (signed char)D_800AC590[4];
        } else {
            if (a0 == 0x12 && D_801094A0 == 1 && D_8010B2DC[D_800FFC00] == 1) {
                a0 = 0x1C;
            } else if (a0 == 0x15 && D_800FFC00 == 1) {
                a0 = 0x22;
            } else if (a0 == 0x25 && D_800FFC00 == 1) {
                a0 = 0x26;
            } else if (a0 == 0x1F && D_80109490 == 0) {
                a0 = 0x20;
            }
        }
        if (a0 >= 0) {
            D_800AC590[4] = a0;
            func_80032A14(D_800F3AFC[4] + 10, D_800F3AFC[5] + 0x28, D_800AC590, a3);
        }
    }
    func_8007D448(a4, ((s0 & 2) > 0) & (*(short *)(D_800AC590 + 0x12) >= 6));
    func_8007D560(a4, (s0 & 4) > 0);
    if (s0 & 8) func_8007D65C(D_800F4D3C);
}

void func_8007D448(int a0, int a1) {
    int i = 0;
    unsigned char *p, *q;
    int v;
    do {
        if (a1 == 1) {
            p = (unsigned char *)(i * 0x24 + (int)D_800F4D80 + 0x558);
            if (p[0x14] == 5) { p[0x14] = 0; goto tail; }
        }
        if (a1 == 0) {
            q = (unsigned char *)(i * 0x24 + (int)D_800F4D80 + 0x558);
            v = q[0x14];
            if (v == 2) { q[0x14] = 3; goto tail; }
            if (v != 0) goto tail;
            return;
        }
tail:
        func_8003EC88(i + 0x26);
        if (i == a0) *(char *)(i * 0x24 + (int)D_800F4D80 + 0x564) = 2;
        else *(char *)(i * 0x24 + (int)D_800F4D80 + 0x564) = 0;
        i++;
    } while (i < 2);
}

void func_8007D558(void) {
}

void func_8007D560(int a0, int a1) {
 int n, i;
 if (D_800FFC18 == 0) n = 4; else if (D_800FFC18 == 6) n = 3; else n = 2;
 i = 0;
 if (n != 0) {
  do {
   func_8003EC88(i + 0x22);
   if (a1 == 1) {
    if (i == a0) *(D_800F4D80 + i*0x24 + 0x4D4) = 2;
    else *(D_800F4D80 + i*0x24 + 0x4D4) = 0;
   } else {
    signed char *p = (signed char *)(i*0x24 + (int)D_800F4D80) + 0x4C8;
    if (p[0xC] == 2) p[0xC] = 1;
   }
   i++;
  } while (i < n);
 }
}

void func_8007D65C(int a0)
{
  int new_var;
  int new_var2;
  func_8001F400(0x28, 0xA0, D_80011680, 0x414);
  func_8001F400(0x68, 0xA0, D_80011688, 0x414);
  func_8001F400(0xC2, 0xA0, D_80011690, 0x414);
  new_var2 = *((int *) 0x1F800000);
  *((int *) 0x1F800000) = (new_var = func_8001FD20(D_800ACC14 + 0x74, new_var2, 0x20, (a0 << 5) + 0xB2, 0x104, 0xC, 0x80, 0x20, 0x20));
}

void func_8007D710(void) {

}
