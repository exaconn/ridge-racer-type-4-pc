/* RODATA 0x8001160c 0x74 */
/* Candidate module: menu transitions, 8007BD18..8007BFF4.
 * Contiguous functions indexed by the same menu state and entry table. */
#include "rr/tables.h"

struct E { char pad[4]; unsigned char a; char b; unsigned char c; signed char d; };

typedef struct { char b[0x17]; signed char b17; char r[0x2C - 0x18]; } w8c5S;

typedef struct { char pad[4]; unsigned char b; char pad2[3]; } E8;

typedef struct { s16 a, b; } P2;

typedef struct 
{
  char b[0x2C];
} W10_R;

typedef struct 
{
  char p[0x10];
  short chk;
} W10_H;

extern s32 func_8004F24C(), func_80078E1C(), func_80078E40();
extern s32 func_8007A310(), func_8007A9CC();
extern RrEntry800A4258 D_800A4258[];
extern RrMenuChoice D_800A44F4[];
extern s32 D_800A4510[];
extern s16 D_800A4514[];
extern RrMenuChoice D_800A4520[];
extern s8 *D_800AC590;
extern s16 D_800AC594;
extern s32 D_800F2DE4;
extern s16 D_800F3A1C;
extern u8 D_800F3A24[];
extern RrMenuItemState5C D_800F3A38[];
extern s16 D_800FFC00;
extern u16 D_80109458;
extern s16 D_8010948C;
extern unsigned char D_800A4524[][4];
extern unsigned short D_800F3AF8;
extern unsigned short D_800F4D8A;
extern unsigned short D_800FFC18;
extern short D_80109478[];
extern short D_8010948E;
extern short D_801094A0;
extern int func_8002B330();
extern int func_8004F24C();
extern int func_80053AF4();
extern int func_8003A198();
extern int func_800207D4();
extern int func_8003B934();
extern int func_8002A604();
extern int func_80053CB0();
extern int func_80114920();
extern int func_80115598();
extern int func_801161A8();
extern int func_8011512C();
extern w8c5S D_800F2E30[];
extern w8c5S D_8010B258[];
extern struct { char pad[0x500]; unsigned char b500; } *D_800F4D80;
extern struct { char pad[0x10]; short h10; char pad2[0x1A]; short h2C; } *D_800F3AFC;
extern short D_800AC5D0;
extern short D_800F4C6A;
extern short D_800F3A26;
extern short D_800FF688;
extern unsigned short D_800F3A1E;
extern unsigned short D_800F4D44;
extern int D_800ACC14;
extern int D_800AC5D4;
extern int D_800F4CA8;
extern short D_800F4D3C;
extern int D_801272F0[];
extern unsigned char D_800A44A8[];
extern s16 D_800A4540[][2];
extern u8 D_8010B2DC[];
extern char *D_800ACC0C;
extern int D_8010EFA8;
extern int func_8002B75C();
extern int func_80078178();
extern int func_80078414();
extern int func_80078544();
extern short D_800A45BC;
extern short D_800A45BE;
extern unsigned char D_800ACBCC;
extern char D_800A457C[];
extern char D_800F4D50[];
extern int D_800A45C0;
extern int D_800A45C4;
extern char *D_800A4594[];
extern unsigned char D_800A4478[];
extern short D_80109490;
extern int func_80032A14(), func_8007D448(), func_8007D560(), func_8007D65C();

void func_8007BD18(void);
void func_8007BDF8(void);
void func_8007BEF8(void);
void func_8007BF58(void);
void func_8007BFF4();
void func_8007C308(void);
void func_8007C76C();
void func_8007C848(void);
void func_8007C8D4(void);
void func_8007C944(void);
int func_8007CB44();
int func_8007CDC4();
int func_8007CFC0();
void func_8007D1B8();

void func_8007BD18(void) {
    s32 i = D_800A4258[D_800F3A1C].i;
    s32 a = D_800A44F4[i].a;
    s32 c = D_800A44F4[i].c;
    s32 b = D_800A44F4[i].b;
    s32 r;
    if (c == 1) r = func_8007A310(0, D_800FFC00);
    else r = func_8007A9CC(0, D_800FFC00);
    if (r == 4) {
        D_800F3A1C = a;
    } else if (r == 5) {
        D_800F3A1C = b;
    } else if (r == 6) {
        D_800F3A1C = b;
    }
    if (c == 0xD) {
        if (r == 4) D_8010948C = 1;
    }
}

void func_8007BDF8(void)
{
  s32 a1 = *(D_800A4510 + D_800A4258[D_800F3A1C].i);
  if ((&D_800F3A38[D_800AC594])->a != 0)
  {
    if (a1 == 0)
    {
      func_8004F24C(1);
      if (D_800F3A1C == 0xC)
      {
        D_800F3A1C = D_80109458;
        D_800F3A24[D_800FFC00] = 0x5A;
        func_80078E40(D_800FFC00, 0, 1);
        return;
      }
      D_800F3A1C = 0;
    }
  }
  else
    if ((&D_800F3A38[D_800AC594])->b != 0)
  {
    if (a1 == 0)
    {
      func_8004F24C(2);
      D_800F3A1C = 0;
    }
  }
}

void func_8007BEF8(void) {
    s32 i = D_800A4258[D_800F3A1C].i;
    s32 v = D_800A4514[i * 2];
    if (D_800AC590[6] == 4) {
        D_800F3A1C = v;
        D_800F2DE4 -= 1;
    }
}

void func_8007BF58(void) {
    s32 k;
    s32 i;
    s32 a, b;
    for (k = 0x63; k >= 0; k--) ;
    i = D_800A4258[D_800F3A1C].i;
    a = D_800A4520[i].a;
    b = D_800A4520[i].c;
    if (func_80078E1C(D_800FFC00, 0) == 1) {
        D_800F3A1C = a;
        return;
    }
    D_800F3A1C = b;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800A4258 (*(struct E (*)[])&D_800A4258)
#define D_800F3A38 (*(char (*)[])&D_800F3A38)
#define func_80078E40 ((int (*)())func_80078E40)

void func_8007BFF4()
{
    int arr[4];
    int i, c, flag, n;
    struct E *base, *p;
    unsigned char *q;
    char *r, *b3;
    signed char sb;

    D_801094A0 = 0;
    D_800F3AF8 = D_800FFC18;
    switch ((short)D_800F3AF8) {
    case 0: D_800F3A1C = 0; break;
    case 1: case 5: D_800F3A1C = 0x20; break;
    case 2: D_800F3A1C = 0x21; break;
    case 3: D_800F3A1C = 0x18; break;
    case 6: D_800F3A1C = 0x2A; D_800F3AF8 = 0x10; break;
    case 7: D_800F3A1C = 0x2B; D_800F3AF8 = 0x10; break;
    }
    base = D_800A4258;
    p = base + D_800F3A1C;
    i = p->a;
    c = p->c;
    D_8010948E = c;
    if (D_800F4D8A == 0) {
        sb = p->d;
        if (sb >= 0) {
            D_80109478[c] = sb;
        }
        D_800F4D8A = 1;
    }
    flag = 0;
    q = D_800A4524[i];
    i = 0;
    arr[0] = q[0];
    arr[1] = q[1];
    arr[2] = q[2];
    arr[3] = q[3];
    n = 4;
    for (; i < 4; i++) {
        if (arr[i] == 0xFF) { n = i; break; }
        if (i == 1 && (arr[1] & 0x80)) {
            flag = 1;
            arr[1] ^= 0x80;
        }
    }
    if (flag) {
        func_8002B330(((unsigned short)D_800AC594 & 1) << 2, 2, D_80109478 + D_8010948E, n, 0);
    } else {
        func_8002B330((((unsigned short)D_800AC594 & 1) << 2) | 1, 2, D_80109478 + D_8010948E, n, 0);
    }
    b3 = D_800F3A38;
    r = b3 + D_800AC594 * 0x5C;
    if (*(int *)(r + 0x28) != 0) {
        func_8004F24C(1);
        D_800F4D8A = 0;
        D_800F3A1C = arr[D_80109478[D_8010948E]];
        if (D_800F3A1C != 0x43) {
            D_800F3A24[D_800FFC00] = 0x5A;
            func_80078E40(D_800FFC00, 0, 1);
        }
    } else if (*(int *)(r + 0x2C) != 0) {
        func_8004F24C(2);
        D_800F4D8A = 0;
        D_800F3A1C = arr[n - 1];
    }
}

#undef D_800A4258
#undef D_800F3A38
#undef func_80078E40

/* legacy typing of shared globals in this part of the unit */
#define D_800FFC18 (*(short (*))&D_800FFC18)

void func_8007C308(void) {
    int i;
    char *q;
    if (D_800F4D80->b500 == 2) {
        for (i = 0; i < 4; i++) {
            q = (char *)(i * 0x24 + (int)D_800F4D80) + 0x4C8;
            if (q[0x14] == 2) {
                q[0x14] = 3;
            }
        }
        D_800F3AFC->h10 = 4;
        if (D_800FFC18 == 3 || D_800FFC18 == 6 || D_800FFC18 == 7) {
            D_800F3AFC->h2C = 4;
        }
        D_800AC5D0 = 0x40;
        if (D_800FFC18 == 0) {
            func_80053AF4(0, 0x3C);
        }
        func_8003A198();
        D_800F4CA8 = 2;
        return;
    }
    if (D_800AC5D0 > 0) {
        D_800AC5D0 = D_800AC5D0 - 1;
    }
    func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) << 2, 0x45);
    if (func_8003B934() != 1) return;
    if (D_800AC5D0 > 0) return;
    if (D_800F4D80->b500 != 5) return;
    if (D_800F3AFC->h10 != 0xB) return;
    for (i = 0; i < 3; i++) {
        D_800F2E30[i] = D_8010B258[i];
    }
    func_8002A604();
    switch (D_800FFC18) {
    case 0:
    default:
    case 4:
        func_80053CB0();
        D_800F4C6A = 1;
        D_800F3A26 = 3;
        D_800F4D8A = 0;
        break;
    case 1:
        if (D_800F3A1E == 0x132) {
            func_80114920();
            D_800F4D8A = 0;
            D_800F4C6A = D_800F3A1E >> 8;
            D_800F3A26 = D_800F3A1E & 0xFF;
        }
        break;
    case 2:
        if ((unsigned)(D_800F4D44 - 2) < 2) {
            func_80115598(D_800AC5D4);
            D_800F4C6A = 1;
            D_800F3A26 = 0x38;
            D_800F4D8A = 0;
        } else if ((short)D_800F4D44 == 4) {
            if (D_800FF688 != -1) {
                func_801161A8(D_800AC5D4);
                D_800F4C6A = 1;
                D_800F3A26 = 0x3C;
            } else {
                func_80053CB0();
                D_800F4C6A = 1;
                D_800F3A26 = 0x2D;
            }
            D_800F4D8A = 0;
        }
        break;
    case 3:
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
        D_800F4D8A = 0;
        break;
    case 5:
        func_8011512C();
        D_800F4C6A = 1;
        D_800F3A26 = 0x40;
        D_800F4D8A = 0;
        break;
    case 6:
        if (D_8010948C != 0) {
            D_800F4C6A = 3;
            D_800F3A26 = 3;
        } else {
            D_800F4C6A = 3;
            D_800F3A26 = 2;
        }
        D_800F4D8A = 0;
        break;
    case 7:
        D_800F4C6A = 1;
        D_800F3A26 = 0x27;
        D_800F4D8A = 0;
        break;
    }
}

#undef D_800FFC18

/* legacy typing of shared globals in this part of the unit */
#define D_800FFC18 (*(short (*))&D_800FFC18)

void func_8007C76C()
{
    short t;
    if (D_800F4D8A == 0) {
        D_801272F0[0] = -1;
        D_801272F0[1] = -1;
        D_801272F0[2] = -1;
        D_800F4D3C = 0;
        func_8007CFC0(0, 0);
        D_800F4D8A = 1;
        return;
    }
    if (func_8007CFC0(1, D_800F4D3C) != 0) {
        t = (unsigned short)D_800F4D3C + 1;
        D_800F4D3C = t;
        if (t == 3) {
            if (D_800FFC18 == 7) {
                D_800F3A1C = 0x2B;
            } else {
                D_800F3A1C = 0x2A;
            }
            D_800F4D3C = 0;
            D_800F4D8A = 0;
            return;
        }
        func_8007CFC0(0, t);
    }
}

#undef D_800FFC18

/* legacy typing of shared globals in this part of the unit */
#define D_800A4258 (*(E8 (*)[])&D_800A4258)

void func_8007C848(void) {
 E8 *e = D_800A4258; int s = D_800F3A1C;
 unsigned char *q = D_800A44A8 + e[s].b*4;
 int x = q[0], y = q[1];
 unsigned c = D_800F3A24[D_800FFC00];
 register int d asm("$3"); /* ASM_BARRIER: pin reload value to v1 */
 if (c < 0x10 || x == (d = *(volatile unsigned char *)&D_800F3A24[D_800FFC00])) { D_800F3A1C = y; return; }
 if (d == 0x14 && s == 2) D_800F3A1C = 8;
}

#undef D_800A4258

void func_8007C8D4(void) {
    s32 i = D_800A4258[D_800F3A1C].i;
    s32 a = D_800A4540[i][0];
    s32 b = D_800A4540[i][1];
    if (D_800F3A1C == 0x3B) {
        if (D_8010B2DC[D_800FFC00] == 1) {
            D_800F3A1C = a;
            return;
        }
        D_800F3A1C = b;
        return;
    }
    D_800F3A1C = a;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F2E30 (*(W10_R (*)[])&D_800F2E30)

void func_8007C944(void)
{
  int r;
  int t0;
  int t1;
  W10_R *t2;
  short a1;
  unsigned v1;
  char *a0;
  int new_var;
  char *a2;
  W10_H *a3;
  signed char *e;
  char pad[0x38];
  if (D_800F4D8A == 0)
  {
    func_8007CB44(0);
    D_8010EFA8 = 0;
    D_800F4D8A = 1;
    return;
  }
  r = func_8007CB44(1);
  t1 = 0;
  if (r > 0)
  {
    new_var = 8;
    do
    {
      a1 = 0;
      v1 = 0;
      a0 = ((char *) (t1 * 0x2C)) + ((int) D_800ACC0C);
      do
      {
        if (v1 != new_var)
        {
          a1 += *((unsigned short *) (a0 + 0x1F60));
        }
        v1++;
        a0 = a0 + 2;
      }
      while (v1 < 0x16U);
      a2 = ((char *) (t1 * 0x2C)) + ((int) D_800ACC0C);
      a3 = (W10_H *) (a2 + 0x1F60);
      if (a1 == a3->chk)
      {
        *(t2 = D_800F2E30 + t1) = *((W10_R *) a3);
      }
      else
      {
        e = ((signed char *) D_800F2E30) + (t1 * 0x2C);
        e[0x17] = -1;
      }
      t1++;
    }
    while (t1 < 3);
    D_800F3A1C = 0x18;
    D_800F4D8A = 0;
  }
  else
    if (r < 0)
  {
    for (t1 = 2; t1 >= 0; t1--)
    {
      e = ((signed char *) D_800F2E30) + (t1 * 0x2C);
      e[0x17] = -1;
    }

    D_800F3A1C = 0x18;
    D_800F4D8A = 0;
  }
}

#undef D_800F2E30

/* legacy typing of shared globals in this part of the unit */
#define func_80078E40 ((int (*)())func_80078E40)
#define D_800ACC0C (*(int (*))&D_800ACC0C)

int func_8007CB44(a0)
int a0;
{
    int pad[14];
    int ret;
    int i;
    int r;
    int q;
    int v;
    if (a0 == 0) {
        D_800A45BC = 0;
    }
    ret = 0;
    switch (D_800A45BC) {
    case 0:
        D_800F3A24[1] = 0x5A;
        D_800F3A24[0] = 0x5A;
        func_80078E40(D_800FFC00, 0, 1);
        func_8002B75C(D_800F4D50, D_800A457C);
        D_800A45BC = 1;
        break;
    case 1:
        func_80078E40(D_800FFC00, 0, 0);
        if (D_800F3A24[0] < 0x10U) {
            if (func_80078414(D_800ACBCC, D_800F4D50) == 1) {
                D_800A45BE = 0;
                D_800A45BC = 2;
            } else {
                D_800A45BC = 7;
            }
            break;
        }
        v = D_800F3A24[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            D_800A45BC = 7;
            break;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        D_800A45BC = 3;
        break;
    case 3:
        for (i = 0x63; i >= 0; i--) {
        }
        r = func_80078544(D_800FFC00, 0, D_800F4D50, D_800ACC0C + 0x1F00, 0x100, 0x300);
        if (r == -1) {
            if (D_800A45BE < 3) {
                D_800A45BE = (unsigned short)D_800A45BE + 1;
            } else {
                D_800A45BC = 7;
            }
        } else {
            D_800A45BC = 4;
        }
        break;
    case 4:
        for (i = 0x63; i >= 0; i--) {
        }
        q = func_80078178();
        if (q != 0) {
            if (q == 1) {
                D_800A45BC = 5;
            } else if (D_800A45BE >= 3) {
                D_800A45BC = 7;
            } else {
                D_800A45BC = 2;
                D_800A45BE = (unsigned short)D_800A45BE + 1;
            }
        }
        break;
    case 5:
        ret = 1;
        break;
    case 6:
    case 7:
    default:
        ret = -1;
        break;
    }
    return ret;
}

#undef func_80078E40
#undef D_800ACC0C

/* legacy typing of shared globals in this part of the unit */
#define func_80078E40 ((int (*)())func_80078E40)
#define func_8007A9CC ((int (*)())func_8007A9CC)
#define D_800F3AF8 (*(short (*))&D_800F3AF8)

int func_8007CDC4(a0)
int a0;
{
    int ret;
    int i;
    int v;
    char *p;
    if (a0 == 0) {
        D_800A45C0 = 0;
    }
    ret = 0;
    switch (D_800A45C0) {
    case 0:
        D_800F3A24[1] = 0x5A;
        D_800F3A24[0] = 0x5A;
        func_80078E40(0, 0, 1);
        i = 0x17;
        p = D_800F4D50 + i;
        do {
            *p = 0;
            i--;
            p--;
        } while (i >= 0);
        func_8002B75C(D_800F4D50, D_800A457C);
        D_800F3AF8 = 0x30;
        D_800A45C0 = 1;
        break;
    case 1:
        func_80078E40(0, 0, 0);
        if (D_800F3A24[0] < 0x10U) {
            if (func_80078414(D_800ACBCC, D_800F4D50) == 1) {
                func_8007A9CC(2, 0);
                D_800A45C0 = 2;
            } else {
                goto set1;
            }
            break;
        }
        v = D_800F3A24[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            D_800A45C0 = 3;
            goto set1;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        ret = func_8007A9CC(0, 0);
        if (ret == 4) {
            goto set1;
        } else if (ret == 5) {
            goto set1;
        } else if (ret == 6) {
            goto set1;
        } else {
            ret = 0;
        }
        break;
    case 3:
set1:
        ret = 1;
        break;
    default:
        return ret;
    }
    return ret;
}

#undef func_80078E40
#undef func_8007A9CC
#undef D_800F3AF8

/* legacy typing of shared globals in this part of the unit */
#define func_80078E40 ((int (*)())func_80078E40)
#define func_8007A9CC ((int (*)())func_8007A9CC)
#define D_800F3AF8 (*(short (*))&D_800F3AF8)

int func_8007CFC0(a0, a1)
int a0, a1;
{
    int ret;
    int v;
    if (a0 == 0) {
        D_800A45C4 = 0;
    }
    ret = 0;
    switch (D_800A45C4) {
    default:
    case 0:
        D_800F3A24[1] = 0x5A;
        D_800F3A24[0] = 0x5A;
        func_80078E40(0, 0, 1);
        D_800A45C4 = 1;
        func_8002B75C(D_800F4D50, D_800A4594[a1]);
        D_800F3AF8 = 0x10;
        break;
    case 1:
        func_80078E40(0, 0, 0);
        if (D_800F3A24[0] < 0x10U) {
            if (func_80078414(D_800ACBCC, D_800F4D50) == 1) {
                func_8007A9CC(2, 0);
                D_800A45C4 = 2;
            } else {
                ret = -1;
            }
            break;
        }
        v = D_800F3A24[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            D_800A45C4 = 3;
            ret = -2;
            break;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        ret = func_8007A9CC(0, 0);
        if (ret == 4) {
            D_800A45C4 = ret;
            ret = 2;
        } else if (ret == 5) {
            ret = -1;
        } else if (ret == 6) {
            ret = -1;
        } else {
            ret = 0;
        }
        break;
    case 3:
        ret = -1;
        break;
    case 4:
        ret = 3;
        break;
    }
    return ret;
}

#undef func_80078E40
#undef func_8007A9CC
#undef D_800F3AF8

/* legacy typing of shared globals in this part of the unit */
#define D_800AC590 ((char *)D_800AC590)
#define D_800F3AFC ((short *)D_800F3AFC)
#define D_800FFC18 (*(short (*))&D_800FFC18)

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

#undef D_800AC590
#undef D_800F3AFC
#undef D_800FFC18
