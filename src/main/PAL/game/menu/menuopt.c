/* RODATA 0x8001164c 0x34 */
#include "rr/tables.h"

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

extern RrEntry800A4258 D_800A4258[];
extern s16 D_800A4540[][2];
extern s16 D_800F3A1C;
extern s16 D_800FFC00;
extern u8 D_8010B2DC[];
extern char *D_800ACC0C;
extern W10_R D_800F2E30[];
extern unsigned short D_800F4D8A;
extern int D_8010EFA8;
extern int func_8002B75C();
extern int func_80078178();
extern int func_80078414();
extern int func_80078544();
extern int func_80078E40();
extern short D_800A45BC;
extern short D_800A45BE;
extern unsigned char D_800ACBCC;
extern unsigned char D_800F3A24[];
extern char D_800A457C[];
extern char D_800F4D50[];
extern int func_8007A9CC();
extern int D_800A45C0;
extern short D_800F3AF8;
extern int D_800A45C4;
extern char *D_800A4594[];

void func_8007C8D4(void);
void func_8007C944(void);
int func_8007CB44();
int func_8007CDC4();
int func_8007CFC0();

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
