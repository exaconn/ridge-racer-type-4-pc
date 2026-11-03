#include "types.h"
#include "m2c_macros.h"
#include "game/course/trkintp2.h"
#include "game/demo/opening.h"
#include "game/gfx/gfxdraw.h"
#include "game/sound/sndbgm3.h"
#include "game/sound/sndslot.h"
#include "psyq/libgpu/gpuenv.h"
#include "game/gfx/gfxotag.h"
#include "game/ui/hudmap2.h"

typedef struct { unsigned addr : 24; unsigned len : 8; } P_TAG;

struct DB { char pad[0x70]; unsigned int ot[1]; };

typedef struct {
    int x, y, z, w;
} V4;

typedef struct {
    short x, y, z, w;
} V8;

typedef struct {
    V4 f0;
    V8 f10;
    int f18;
    int f1C;
    int f20;
    int f24;
} Elem28;

typedef struct {
    char pad0[0x10];
    int f10, f14, f18;
} Arg0Struct;

typedef struct {
    char pad0[0x60];
    V4 f60;
    V8 f70;
    int f78;
    int f7C;
    int f80;
    char pad84[0x20];
    int fA4;
} Context650;

extern struct {
    char pad0[0x50];
    int f50[2];
    int f58[2];
    char pad60[0x55C - 0x60];
    char *f55C;
} D_800AC650;
extern short D_800AD520;
extern int D_800F4BF0;
extern int D_800F2FD8;
extern struct DB *D_800ACC14;
extern short D_800F3914;
extern void func_8006EE3C();
extern void func_800701C4();
extern void func_8005F0D0();
extern void func_8006E1AC();
extern void func_80090EC4();
extern void func_800912D4();
extern void func_80091D64();
extern void func_800172DC();
extern int D_800AC6B0;
extern Elem28 (*D_800ACBA4)[16];
extern short D_800F4D44;
extern int func_80096434();
extern M2C_UNK D_800AC6D8;
extern void func_800207D4();
extern void func_8004FED4();
extern void func_800528A8();
extern void func_80052A30();
extern void func_80052B8C();
extern void func_80059E40();
extern int func_80096464();
extern int D_800AC6EC;
extern int D_800ACBF8;
extern signed char D_800AD4D8[];
extern signed char D_800AD528[];
extern unsigned short D_800F3A40;
extern int D_8010718C;

void func_80044EC4(void);
void func_800451E4(int *a);
int func_800452CC(Arg0Struct *a0, int a1, int a2);
void func_80045830(void);
int func_80045868(char *arg0);

void func_80044EC4(void) {
    int sp10[8];
    int *g = (int *)0x1F800000;
    int s5;
    char *s2;
    int s1;

    s5 = g[0x18];
    s2 = D_800AC650.f55C + (D_800AD520 << 7);
    func_80044920(s2, sp10);
    func_8006EE3C(0x1F80007C, s2, sp10);
    s1 = func_80044B24(sp10, s2);
    if (s1 > 0) {
        func_8009323C(D_800AC650.f50[D_800F4BF0], 0x15F);
        g[1] = D_800AC650.f50[D_800F4BF0];
        g[0x18] = s1 >> 7;
        if (s1 >> 7 >= 6) {
            g[0x18] = 5;
        }
        func_800701C4(8);
        func_8006EE3C(0x1F80007C, s2, sp10);
        g[0x1E] = 0;
        func_8005F0D0(0x1F800000, D_800F2FD8 < 1, 0, 0);
        s1 >>= s5;
        s1 += 0x6E;
        if (s1 > 0) {
            func_80044BA0(s1 + 1);
            ((P_TAG *)g[1])->addr = ((P_TAG *)&D_800ACC14->ot[s1])->addr;
            ((P_TAG *)&D_800ACC14->ot[s1])->addr = g[1] + 0x578;
        }
    }
    if (D_8009FE60[D_800F3914] == 7) {
        s2 = D_800AC650.f55C + (D_800AD520 << 7) + 0x40;
        func_80044920(s2, sp10);
        func_8006EE3C(0x1F80007C, s2, sp10);
        s1 = func_80044B24(sp10, s2);
        if (s1 > 0) {
            func_8009323C(D_800AC650.f58[D_800F4BF0], 0x15F);
            g[1] = D_800AC650.f58[D_800F4BF0];
            g[0x18] = s1 >> 7;
            if (s1 >> 7 >= 6) {
                g[0x18] = 5;
            }
            func_800701C4(9);
            func_8006EE3C(0x1F80007C, s2, sp10);
            *(int *)0x1F800078 = 0;
            func_8005F0D0(0x1F800000, D_800F2FD8 < 1, 0, 0);
            s1 >>= s5;
            s1 += 0x6E;
            if (s1 > 0) {
                func_80044BA0(s1 + 1);
                ((P_TAG *)g[1])->addr = ((P_TAG *)&D_800ACC14->ot[s1])->addr;
                ((P_TAG *)&D_800ACC14->ot[s1])->addr = g[1] + 0x578;
            }
        }
    }
    g[0x18] = s5;
    g[1] = (int)D_800ACC14 + 0x70;
}

void func_800451E4(int *a) {
    int m[8];
    unsigned short v[3];
    short r[3];
    int t[3];
    int *s = (int *)0x1F800000;
    s[6] = -*(short *)((char *)a + 0x10);
    s[7] = *(short *)((char *)a + 0x12);
    s[8] = *(short *)((char *)a + 0x14);
    v[0] = *(short *)((char *)a + 0x10);
    v[1] = *(short *)((char *)a + 0x12);
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -*(unsigned short *)((char *)a + 0x1C);
    func_80091D64(v, m);
    func_80090EC4(m, r, t);
    s[2] = a[0] + t[0];
    s[3] = a[1] + t[1];
    s[4] = a[2] + t[2];
    func_800912D4(a[8]);
    func_8006E1AC();
}

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(Context650 (*))&D_800AC650)

int func_800452CC(Arg0Struct *a0, int a1, int a2) {
    int pad[4];
    V4 sp20;
    V4 sp30;
    int s0;
    int s3;
    int i;
    Elem28 *s2;
    int t1;
    int t2;

    s0 = -1;
    s2 = D_800ACBA4[a2];
    for (i = 1; i < 0x10; i++) {
        if (s2[i].f24 * 5 >= a1) {
            s0 = i;
            s3 = s0 - 1;
            break;
        }
    }
    if (s0 < 0) {
        return 1;
    }
    if (D_800AC650.fA4 != s3) {
        func_800172DC();
        D_800AC650.fA4 = s3;
    }
    t1 = (s2[s0].f24 - s2[s3].f24) * 5;
    t2 = a1 - s2[s3].f24 * 5;
    if (s2[s3].f18 == 0) {
        sp20.x = a0->f10;
        sp20.y = a0->f14;
        sp20.z = a0->f18;
    } else {
        sp20 = s2[s3].f0;
    }
    if (s2[s0].f18 == 0) {
        sp30.x = a0->f10;
        sp30.y = a0->f14;
        sp30.z = a0->f18;
    } else {
        sp30 = s2[s0].f0;
    }
    if (t1 > 0) {
        D_800AC650.f60.x = ((sp30.x - sp20.x) * t2) / t1 + sp20.x;
        D_800AC650.f60.y = ((sp30.y - sp20.y) * t2) / t1 + sp20.y;
        D_800AC650.f60.z = ((sp30.z - sp20.z) * t2) / t1 + sp20.z;
        D_800AC650.f70.x = ((s2[s0].f10.x - s2[s3].f10.x) * t2) / t1 + s2[s3].f10.x;
        D_800AC650.f70.y = ((s2[s0].f10.y - s2[s3].f10.y) * t2) / t1 + s2[s3].f10.y;
        D_800AC650.f70.z = ((s2[s0].f10.z - s2[s3].f10.z) * t2) / t1 + s2[s3].f10.z;
        D_800AC650.f7C = ((s2[s0].f1C - s2[s3].f1C) * t2) / t1 + s2[s3].f1C;
        D_800AC650.f80 = ((s2[s0].f20 - s2[s3].f20) * t2) / t1 + s2[s3].f20;
    } else {
        D_800AC650.f60 = sp30;
        D_800AC650.f70 = s2[s0].f10;
        D_800AC650.f7C = s2[s0].f1C;
        D_800AC650.f80 = s2[s0].f20;
    }
    func_800451E4(&D_800AC6B0);
    if (D_800F4D44 == 1) {
        func_80044B5C();
        func_80044EC4();
    }
    return 0;
}

#undef D_800AC650

void func_80045830(void) {
    func_80096434(&D_800AC6D8, 0x20);
    M2C_FIELD(&D_800AC6D8, s32 *, 0x1C) = -1;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(int (*)[])&D_800AC650)
#define D_800AC6B0 (*(char (*)[])&D_800AC6B0)
#define D_800AC6D8 (*(int (*))&D_800AC6D8)
#define D_800ACC14 ((char *)D_800ACC14)

int func_80045868(char *arg0)
{
  int i;
  int s0;
  int *p;
  signed char *q;
  char *r;
  char *pk;
  char *pk2;
  char *ot;
  char *ot2;
  int v;
  int v1;
  int v2;
  int c;
  int *g;
  int *g2;
  int *g1;
  int *gB;
  int *gC;
  int *gL;
  int *gF;
  int *gD;
  int *gS;
  int *gT;
  s0 = 0;
  do
  {
    func_80050028(s0, 1);
    s0++;
  }
  while (s0 < 4);
  g1 = D_800AC650;
  if ((*((int *) (((char *) g1) + 0x90))) != 0)
  {
    func_800207D4(D_800ACC14 + 0x70, (*((int *) (((char *) g1) + 0x8C))) * 10, 0x25);
    v1 = (*((int *) (((char *) g1) + 0xA0))) - 8;
    *((int *) (((char *) g1) + 0xA0)) = v1;
    if (v1 < 0)
    {
      *((int *) (((char *) g1) + 0xA0)) = 0;
    }
    func_80051538(0, (short) (*((int *) (((char *) g1) + 0xA0))), (short) (*((int *) (((char *) g1) + 0xA0))));
    *((int *) (((char *) g1) + 0x8C)) = (*((int *) (((char *) g1) + 0x8C))) + 1;
  }
  else
  {
    v2 = (*((int *) (((char *) g1) + 0xA0))) + 8;
    *((int *) (((char *) g1) + 0xA0)) = v2;
    if (v2 >= 0x61)
    {
      *((int *) (((char *) g1) + 0xA0)) = 0x60;
    }
    func_80051538(0, (short) (*((int *) (((char *) g1) + 0xA0))), (short) (*((int *) (((char *) g1) + 0xA0))));
  }
  gB = D_800AC650;
  s0 = gB[0x88 / 4];
  if (s0 < 0xC)
  {
    if (s0 == 1)
    {
      func_8005A90C();
      if (gB[0x94 / 4] == s0)
      {
        func_80052A30(0);
      }
      {
        int two;
        s0 = 0;
        two = 2;
        q = D_800AD528;
        p = gB;
        do
        {
          s0++;
          p[0xA8 / 4] = q[0x2A];
          __asm__ volatile("" : "+r"(q) : "r"(p)); /* ASM_BARRIER: prevent CSE of q + 0x2A and prioritize p for a0 */
          q[0x2A] = two;
          q += 4;
          p++;
        }
        while (s0 < 2);
      }
    }
    {
      int x, d;
      x = D_800AC6D8 << 8;
      d = x / 12;
      s0 = 0x100;
      __asm__("" : "+r"(s0) : "r"(x)); /* ASM_BARRIER: keep 0x100 in s0 and prioritize x in v0 */
      s0 -= d;
    }
    func_80059E40(0x1C2);
    {
      int w, low, high;
      low = 0xFFFFFF;
      high = 0xFF000000;
      pk = *((char **) 0x1F800000);
      pk[3] = 3;
      pk[7] = 0x62;
      pk[4] = s0;
      pk[5] = s0;
      pk[6] = s0;
      ot = D_800ACC14;
      __asm__("" : "+r"(pk) : "r"(ot), "r"(low)); /* ASM_BARRIER: order ot load before pk load and prioritize low */
      w = *((int *) pk);
      *((short *) (pk + 0xC)) = 0x140;
      *((short *) (pk + 8)) = 0;
      *((short *) (pk + 0xA)) = 0;
      *((short *) (pk + 0xE)) = 0xF0;
      *((int *) pk) = (w & high) | ((*((int *) (ot + 0x70))) & low);
      *((int *) (ot + 0x70)) = ((*((int *) (ot + 0x70))) & high) | (((int) pk) & low);
      pk += 0x10;
      *((char **) 0x1F800000) = pk;
      pk[3] = 1;
      ot2 = D_800ACC14;
      __asm__("" : "+r"(pk) : "r"(ot2)); /* ASM_BARRIER: order ot2 load before pk load */
      w = *((int *) pk);
      *((int *) (pk + 4)) = 0xE1000245;
      *((int *) pk) = (w & high) | ((*((int *) (ot2 + 0x70))) & low);
      *((int *) (ot2 + 0x70)) = ((*((int *) (ot2 + 0x70))) & high) | (((int) pk) & low);
      *((char **) 0x1F800000) = pk + 8;
    }
  }
  gC = D_800AC650;
  if (gC[0x94 / 4] == 0)
  {
    if ((D_800F4D44 == 1) && (D_800AD4D8[D_800AD4D8[0x17] + 8] >= 3))
    {
      s0 = 1;
      gL = gC;
      gL[0x98 / 4] = 0;
      r = (((char *) gL[0x554 / 4]) + (D_800AD520 * 0x280)) + 0x28;
      do
      {
        v = *((int *) (r + 0x24));
        if (gL[0x98 / 4] < v)
        {
          gL[0x98 / 4] = v;
        }
        s0++;
        r += 0x28;
      }
      while (s0 < 0x10);
      gF = D_800AC650;
      gF[0x94 / 4] = 1;
      gF[0x98 / 4] = gF[0x98 / 4] * 5;
    }
    else
    {
      gC[0x94 / 4] = 2;
    }
  }
  gD = D_800AC650;
  switch (gD[0x94 / 4])
  {
    case 1:
      func_800452CC(arg0, gD[0x88 / 4], D_800AD520);
      if (D_800F3A40 & 0x800)
    {
      if (gD[0x88 / 4] >= 0x1A)
      {
        if (gD[0x90 / 4] == 0)
        {
          gD[0x98 / 4] = gD[0x88 / 4] + 0x19;
        }
      }
    }
      gS = D_800AC650;
      if ((gS[0x98 / 4] - gS[0x88 / 4]) < 0x1A)
    {
      if (gS[0x90 / 4] == 0)
      {
        gS[0x90 / 4] = 1;
        func_80052B8C(0, 8);
      }
    }
      gT = D_800AC650;
      gT[0x88 / 4]++;
      if (gT[0x88 / 4] >= gT[0x98 / 4])
    {
      gT[0x88 / 4] = gT[0x98 / 4];
      gT[0x9C / 4] = 1;
    }
      D_8010718C = 0;
      break;

    case 2:
      gD[0x88 / 4]++;
      if (gD[0x88 / 4] >= 0x49)
    {
      gD[0x9C / 4] = 1;
      *((unsigned short *) (arg0 + 0x1E4)) = (*((unsigned short *) (arg0 + 0x1E4))) | 0x100;
    }
      if (gD[0x9C / 4] == 0)
    {
      *((short *) (((char *) D_800AC650) + 0x30)) = 0;
      *((short *) (((char *) D_800AC650) + 0x32)) = 0;
      *((short *) (((char *) D_800AC650) + 0x34)) = -0x100;
      func_800439BC(arg0, ((char *) D_800AC650) + 0x30, ((char *) D_800AC650) + 0x60);
      *((short *) (((char *) D_800AC650) + 0x74)) = 0;
      *((int *) (((char *) D_800AC650) + 0x80)) = 0x1D6;
      *((short *) (((char *) D_800AC650) + 0x70)) = (((*((int *) (((char *) D_800AC650) + 0x88))) * 0x190) / 73) - 0x280;
      v = (*((short *) (((char *) D_800AC650) + 0x72)) = ((-((*((int *) (((char *) D_800AC650) + 0x88))) * 0xB00)) / 73) + 0x600);
      *((int *) (((char *) D_800AC650) + 0x7C)) = ((-((*((int *) (((char *) D_800AC650) + 0x88))) << 9)) / 73) + 0x400;
      if (((short) v) < (-0x400))
      {
        *((short *) (((char *) D_800AC650) + 0x72)) = -0x400;
      }
    }
    else
    {
      *((short *) (((char *) D_800AC650) + 0x30)) = 0;
      *((short *) (((char *) D_800AC650) + 0x32)) = 0;
      *((short *) (((char *) D_800AC650) + 0x34)) = -0x100;
      func_800439BC(arg0, ((char *) D_800AC650) + 0x30, ((char *) D_800AC650) + 0x60);
      *((short *) (((char *) D_800AC650) + 0x70)) = -0xF0;
      *((short *) (((char *) D_800AC650) + 0x72)) = -0x400;
      *((int *) (((char *) D_800AC650) + 0x7C)) = 0x200;
      *((short *) (((char *) D_800AC650) + 0x74)) = 0;
      *((int *) (((char *) D_800AC650) + 0x80)) = 0x1D6;
    }
      func_800451E4(D_800AC6B0);
      g2 = (int *) (D_800AC6B0 - 0x60);
      if (g2[0x88 / 4] >= 0x33)
    {
      g2[0x90 / 4] = 1;
    }
      break;

  }

  g = D_800AC650;
  if (g[0x9C / 4] != 0)
  {
    func_8004FED4();
    v = g[0x9C / 4];
    D_800ACBF8 = 1;
    if (v == 1)
    {
      if (g[0x94 / 4] == v)
      {
        if (D_800F765C == 0)
        {
          func_800528A8(0, (func_80096464() % 14) + 0xB);
        }
        else
        {
          func_800528A8(0, D_800A0C9C[D_800F765C - 1]);
        }
      }
      D_800AC6EC = 2;
    }
    s0 = 0;
    p = D_800AC650;
    q = D_800AD528;
    do
    {
      q[0x2A] = *(((unsigned char *) p) + 0xA8);
      p++;
      q += 4;
      s0++;
    }
    while (s0 < 2);
  }
  else
  {
    D_800ACBF8 = 0;
  }
  return D_800AC6EC;
}

#undef D_800AC650
#undef D_800AC6B0
#undef D_800AC6D8
#undef D_800ACC14
