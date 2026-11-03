/* RODATA 0x80011068 0x20 */
#include "types.h"
#include "m2c_macros.h"
#include "game/data/cdfile2.h"
#include "game/course/trkres2.h"
#include "game/data/trkld1.h"
#include "game/gfx/gfxotag.h"
#include "game/course/trkres2.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"

extern int D_800A02BC;
extern int D_800A029C[];
extern short D_800F4D44;
extern int func_80070108();
extern int func_800436E8();
extern int func_80043608();
extern int func_80070108();
extern int func_800436E8();
extern int func_80050BCC();
extern struct { int a, b, c, d; } D_8010C200;
extern int func_80050978();
extern int func_8004ED38();
extern int D_800A0228[];
extern struct { char pad[0x2858]; char *p[1]; } D_800F4DB8;
extern char *D_8010C210[2];
extern int D_8010C218;
extern s32 D_800ACC18;
extern int D_800AC650[];
extern int D_800ACBBC;

int func_8004D628();
void func_8004D7B4(s32 arg0);
s32 func_8004D7C8(void);
int func_8004D7D8();
int func_8004D994();
int func_8004DB94(int a0, int a1);
int func_8004DCCC(int a0);
int func_8004DDF8();
int func_8004DEBC(int a0, int a1);
void func_8004E008(void);
int func_8004E03C(char *a0);
s32 func_8004E184(void);
s32 func_8004E18C(void);
int func_8004E194(void);

int func_8004D628(a0, a1, a2)
int a0;
int a1;
int a2;
{
  int v;
  int t;
  int e;
  e = 0x44;
  v = 0x1E;
  switch (D_800A0220)
  {
    case 0:
      switch (D_800F4D44)
    {
      case 1:
        D_800A02BC = func_8001223C(2, D_800A029C[a0] + (a2 + v));
        break;

      case 3:
        D_800A02BC = func_8001223C(2, (a0 * 4) + (a2 + e));
        break;

      case 5:
        D_800A02BC = func_8001223C(2, a0 + 0x74);
        break;

      case 6:
        D_800A02BC = func_8001223C(2, (a0 * 2) + 0x64);
        break;

      default:
        D_800A0220 = 0;
        return 1;

    }

    case 1:
      v = func_8001292C(D_800A02BC, D_8010C1F0);
      if (v != 0)
    {
      D_800A0220++;
    case 2:
      t = func_80012A58();
      if (t != 0)
      {
        func_8004D61C(D_8010C1F0);
        v = func_8004D4BC(t);
        D_800A0220 = 0;
        D_8010C1F0 += v;
        return 1;
      }
      v = 0;
    }
      return v;
    default:
      return 0;
  }
}

void func_8004D7B4(s32 arg0) {
    D_8010C1F0 = arg0;
    D_800A0220 = 0;
}

s32 func_8004D7C8(void) {
    return D_8010C1F0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((int *)D_8010C1F0)
#define D_800AC650 (*(struct { char pad[0x24C]; char *p; char pad2[0x560 - 0x250]; char *r560; } (*))&D_800AC650)

int func_8004D7D8(a0, a1) int a0, a1;
{
    int v, t, e;
    e = 0x86;
    switch (D_800A0220) {
    case 0:
        v = func_8001292C(func_8001223C(2, (a0 * 4) + (a1 + e)), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 1:
        t = func_80012A58();
        if (t != 0) {
            func_800436E8((char *)D_8010C1F0 + D_8010C1F0[1]);
            func_80043608((char *)D_8010C1F0 + D_8010C1F0[2]);
            D_800A0220++;
        }
        return 0;
    case 2:
        v = func_8001292C(func_8001223C(2, 0x96), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 3:
        t = func_80012A58();
        if (t != 0) {
            func_80070108((char *)D_8010C1F0 + D_8010C1F0[1], 8);
            func_80012A68((char *)D_8010C1F0 + D_8010C1F0[2]);
            D_800AC650.r560 = (char *)D_8010C1F0 + D_8010C1F0[2];
            ((int *)D_800AC650.p)[0x138 / 4] = (int)((char *)D_8010C1F0 + D_8010C1F0[3]);
            v = func_8004D4BC(t);
            D_800A0220 = 0;
            D_8010C1F0 = (int *)((char *)D_8010C1F0 + v);
            return 1;
        }
        return 0;
    default:
        return 0;
    }
out:
    return v;
}

#undef D_8010C1F0
#undef D_800AC650

int func_8004D994(a0, a1) int a0, a1;
{
    int v, t;
    switch (D_800A0220) {
    case 0:
        if (D_800F4D44 != 1) return 1;
        v = func_8001292C(func_8001223C(2, a1 * 2 + 0x7D), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
        goto c1;
    case 2:
        v = func_8001292C(func_8001223C(2, a1 * 2 + 0x7C), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 3:
        t = func_80012A58();
        if (t != 0) {
            func_80070108(D_8010C1F0, 8);
            v = func_8004D4BC(t);
            D_8010C1F0 += v;
            if (D_8009FE60[a0] < 7) {
                D_800A0220 = 0;
                return 1;
            }
            goto inc;
        }
        return 0;
    case 4:
        v = func_8001292C(func_8001223C(2, 0x85), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 1:
    case 5:
    c1:
        t = func_80012A58();
        if (t != 0) {
            func_800436E8(D_8010C1F0);
        inc:
            D_800A0220++;
        }
        return 0;
    case 6:
        v = func_8001292C(func_8001223C(2, 0x84), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 7:
        t = func_80012A58();
        if (t != 0) {
            func_80070108(D_8010C1F0, 9);
            v = func_8004D4BC(t);
            D_8010C1F0 += v;
            D_800A0220 = 0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
out:
    return v;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)

int func_8004DB94(int a0, int a1) {
    int v;
    if (a0 < 0) return 1;
    switch (D_800A0224) {
    case 0:
        v = func_8001292C(func_8001223C(2, a0 + 3, a0), D_8010C1F0);
        if (v != 0) {
            D_800A0224 += 1;
    case 1:
            if (func_80012A58() != 0) {
                D_8010C200.c = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 4));
                D_8010C200.a = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
                D_8010C200.b = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 0xC));
                D_8010C200.d = a1;
                D_8010C1F0 += func_8004D4BC(*(int *)(D_8010C1F0 + 0xC));
                D_800A0224 += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        func_80050BCC(&D_8010C200);
        D_800A0224 = 0;
        return 1;
    default:
        return 0;
    }
}

#undef D_8010C1F0

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)

int func_8004DCCC(int a0) {
    int v;
    if (a0 < 0) return 1;
    switch (D_800A0224) {
    case 0:
        v = func_8001292C(func_8001223C(2, a0 + 0xC), D_8010C1F0);
        if (v != 0) {
            D_800A0224 += 1;
    case 1:
            if (func_80012A58() != 0) {
                D_8010C200.c = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 4));
                D_8010C200.a = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
                D_8010C200.b = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 0xC));
                D_8010C200.d = 0;
                D_8010C1F0 += func_8004D4BC(*(int *)(D_8010C1F0 + 0xC));
                D_800A0224 += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        func_80050978(&D_8010C200);
        D_800A0224 = 0;
        return 1;
    default:
        return 0;
    }
}

#undef D_8010C1F0

int func_8004DDF8(a0, a1, a2)
int a0, a1, a2;
{
    switch (D_800A0220) {
    case 0:
        if (func_8004DB94(a0, 0) == 0) return 0;
        if (a1 >= 0) D_800A0220 = 1;
        else D_800A0220 = 2;
        break;
    case 1:
        if (func_8004DB94(a1, 1)) {
            D_800A0220 = 0;
            return 1;
        }
        break;
    case 2:
        if (func_8004DCCC(a2)) {
            D_800A0220 = 0;
            return 1;
        }
    }
    return 0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)

int func_8004DEBC(int a0, int a1) {
    int s0 = 0;
    int *s2 = D_800A0228 + a0 * 3;
    int v, t;
    char *v1, *v0;
    switch (D_800A0224) {
    case 0:
        v = func_8001292C(func_8001223C(2, s2[0]), D_8010C1F0);
        if (v != 0) {
            D_800A0224 += 1;
    case 1:
            if (func_80012A58() != 0) {
                if (a1 > 0) s0 += a1;
                v1 = (char *)(s0 * 4 + (int)D_8010C1F0);
                D_8010C210[0] = D_8010C1F0 + *(int *)(v1 + 4);
                v0 = D_8010C1F0 + *(int *)(v1 + 8);
                D_8010C210[1] = v0;
                s0 = v0 - D_8010C1F0;
                func_8004ED38(D_8010C210, s2[1], s2[2], D_8010C1F0);
                if (a1 > 0) {
                    D_800F4DB8.p[D_8010C218] = D_8010C1F0 + *(int *)(D_8010C1F0 + 4);
                }
                s0 = func_8004D4BC(s0);
                D_800A0224 = 0;
                D_8010C1F0 += s0;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}

#undef D_8010C1F0

void func_8004E008(void) {
    D_800A021C = 0;
    D_800A0220 = 0;
    D_800A0224 = 0;
    D_800A0218 = 0;
    D_8010C1F0 = D_800ACC18;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)
#define D_8010C210 (*(int (*)[2])&D_8010C210)

int func_8004E03C(char *a0) {
    int v;
    int *t = D_800A0228;
    switch (D_800A021C) {
    case 0:
        v = func_8001292C(func_8001223C(2, 0x11), a0);
        if (v != 0) {
            D_800A021C += 1;
    case 1:
            if (func_80012A58() != 0) {
                D_800A021C += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        v = func_8001292C(func_8001223C(2, 0x12), D_8010C1F0);
        if (v != 0) {
            D_800A021C += 1;
    case 3:
            if (func_80012A58() != 0) {
                D_8010C210[0] = (int)a0;
                D_8010C210[1] = (int)D_8010C1F0;
                func_8004ED38(D_8010C210, t[1], t[2]);
                D_800A021C = 0;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}

#undef D_8010C1F0
#undef D_8010C210

s32 func_8004E184(void) {
    return 1;
}

s32 func_8004E18C(void) {
    return 1;
}

int func_8004E194(void) {
    switch (D_800A021C) {
    case 0:
        if (func_8004DEBC(3, 0) != 0) {
            D_800A021C += 1;
        }
        return 0;
    case 1:
        if (func_8004DEBC(D_800ACBBC, 0) != 0) {
            D_800A021C += 1;
        }
        return 0;
    case 2:
        if (func_8004DDF8(D_800AC650[0x570 / 4], D_800AC650[0x574 / 4], D_800AC650[0x578 / 4]) != 0) {
            D_800A021C = 0;
            D_800ACC18 = D_8010C1F0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
