
typedef struct 
{
  short s0;
  char p2[4];
  short s6;
  char p8[2];
  unsigned short sA;
  short sC;
  short sE;
  char p10[2];
  short s12;
  short s14;
  char p16[2];
  short s18;
} R;
typedef struct 
{
  short s0;
  short s2;
  char p4[4];
  short s8[26];
} S;
typedef struct 
{
  char p0[0xC];
  int sC;
  char p10[8];
  S *s18;
  R *s1C;
  char p20[4];
  int s24;
  char p28[8];
  short s30;
  short s32;
  char p34[2];
  short s36;
  short s38;
  char p3A[2];
  short s3C;
  char p3E[2];
  short s40;
  char p42[2];
  short s44;
  char p46[0x40];
  short s86;
  int s88;
  int s8C;
  char p90[0x10];
  int sA0;
  int sA4;
  short sA8;
  short sAA;
  short sAC;
  short sAE;
} Q;
typedef struct 
{
  char p0[0x50];
  int s50;
  int s54;
  int s58;
  char p5C[0x116];
  short s172;
  char p174[0x44];
  short s1B8;
  char p1BA[0x1A];
  short s1D4;
  short s1D6;
  short s1D8;
  char p1DA[4];
  short s1DE;
  int s1E0;
  char p1E4[0x10];
  Q q;
} P;
extern int D_800FF6B0;
extern unsigned char D_800ADAD8[][4];
extern char D_800ABEF8[];
extern short D_800F4C70[];
extern short D_800F2AB4[];
extern int func_80025F84();
extern int func_800904F4();
extern int func_80096464();
extern void func_80024CD8();
extern void func_800252EC();
extern void func_80025954();
void func_8002367C(a0)
P *a0;
{
  Q *q;
  R *r;
  S *s;
  int t0;
  int a2;
  int a1;
  int v0;
  int v1;
  int s1;
  int s3;
  int s4;
  short v;
  short *pf;
  int m1;
  int m2;
  int m3;
  int m4;
  int m5;
  int m6;
  int m7;
  int m8;
  int m9;
  int n1;
  int n2;
  int n3;
  int n4;
  int n5;
  int d1;
  int a;
  int b;
  int off;
  int x;
  int lo;
  int y;
  int k1;
  int k2;
  int k3;
  int k4;
  int k5;
  int k6;
  int fl;
  short t;
  int pad[8];
  int ten;
  q = &a0->q;
  r = q->s1C;
  off = (q->s86 * 0x3C) + 4;
  s = (S *) (((int) q->s18) + off);
  if (D_800FF6B0 < 4)
  {
    q->sAC = D_800ADAD8[q->s24][1];
    q->sAE = D_800ADAD8[q->s24][2];
    if (q->sAC >= 0xFF)
    {
      q->sAC = 0x100;
    }
    if (q->sAE >= 0xFF)
    {
      q->sAE = 0x100;
    }
  }
  else
  {
    q->sAC = (0x400 - a0->s172) / 4;
    q->sAE = a0->s172 / 8;
  }
  if (q->sA8 == 0)
  {
    if (q->sAC >= 0x85)
    {
      q->sA8 = 1;
    }
  }
  else
    if (q->sA8 == 1)
  {
    q->sA8 = 2;
  }
  else
    if (q->sAC < 0x7C)
  {
    q->sA8 = 0;
  }
  if (q->sAA == 0)
  {
    if (q->sAE >= 0x85)
    {
      q->sAA = 1;
    }
  }
  else
    if (q->sAA == 1)
  {
    q->sAA = 2;
  }
  else
    if (q->sAE < 0x7C)
  {
    q->sAA = 0;
  }
  t0 = r->s0;
  if (((short) r->sA) > 0)
  {
    if (a0->s1D8 >= 0x321)
    {
      a2 = a0->s1DE + 0x12C;
    }
    else
    {
      a2 = 0x12C;
    }
    if (a0->s1DE > 0)
    {
      t0 += (a0->s1DE * r->s18) >> 2;
    }
  }
  else
  {
    m1 = q->sAC * 0x64;
    m2 = m1 >> 8;
    if (m1 < 0)
    {
      m1 += 0xFF;
      m2 = m1 >> 8;
    }
    a2 = 0x17C - m2;
    if (q->s3C > 0)
    {
      a2 += q->s3C * 3;
    }
    a2 += ((q->sAE * (0x64 + (((short) r->sA) / 2))) * r->sC) / 256000;
  }
  if (q->sA4 == 1)
  {
    q->s36 = (q->s36 + a2) / 2;
  }
  else
  {
    n2 = q->s44 * a0->s58;
    if (n2 > 0)
    {
      if (r->s12 < n2)
      {
        n2 = r->s12;
      }
      a2 = a2 + ((n2 * r->s14) / 1000);
    }
    m4 = a2 * t0;
    q->s36 = q->s36 + ((((m4 / 1000) - q->s36) * 0x1C) / 32);
  }
  ten = 10;
  s3 = s->s0;
  if (q->s38 == ten)
  {
    a2 = ((q->s88 * s3) - q->sA0) / s3;
    q->s8C = a2 / 10;
    q->s40 = (q->s88 - ((a0->s1D8 * 10000) / ((s->s2 * 0x490) / 160))) / (q->s3C + 1);
  }
  if (q->s38 > 0)
  {
    s1 = q->s8C;
    q->s38 = q->s38 - 1;
    if (q->s32 == 1)
    {
      s3 = ((s->s0 * (ten - q->s38)) + (s[-1].s0 * q->s38)) / 10;
    }
  }
  else
  {
    m5 = (q->s88 * s->s0) - q->sA0;
    q->s32 = 0;
    s1 = m5 / s3;
  }
  if (q->s3C > 0)
  {
    s1 += q->s40;
  }
  k1 = q->s88;
  if (k1 >= 10000)
  {
    a1 = 0x26AC - k1;
  }
  else
  {
    n2 = k1 / 500;
    m7 = k1 - (n2 * 500);
    if ((q->s86 == 1) && (n2 < 8))
    {
      k1 = s->s8[n2] * 3;
    }
    else
    {
      k1 = s->s8[n2];
    }
    a1 = ((k1 * (500 - m7)) + (s->s8[n2 + 1] * m7)) / 500;
  }
  s4 = ((a1 * q->sAC) >> 8) * ((0x100 - q->s30) >> 8);
  if (a0->s1B8 == 0)
  {
    k2 = q->s88;
    n2 = k2 / 256;
    n2 += (((k2 * q->sAE) / 256) * q->s1C->sC) / 32000;
    if (((unsigned) (q->sA4 - 1)) < 2)
    {
      n2 = 0;
    }
    s1 += n2;
  }
  if (a1 > 0)
  {
    if (q->sAC < 0x7F)
    {
      s1 += a1 / 2;
    }
  }
  else
  {
    s1 -= a1 / 2;
  }
  k3 = func_80025F84(a0->s54, a0->s1D4);
  if (k3 > 0x400)
  {
    k3 = 0x800 - k3;
  }
  k6 = k3;
  if (k3 < 0)
  {
    k6 = k3 + 0xFF;
  }
  s1 += k6 >> 8;
  if (q->sA4 == 0)
  {
    k4 = q->sC;
    if ((D_800ADAD8[q->s24][3] & 0xE0) != 0x20)
    {
      k4 = k4 >> 1;
    }
    if (q->sC >= 0)
    {
      s1 += k4 / r->s6;
    }
    else
    {
      s1 -= k4 / r->s6;
    }
  }
  k1 = a0->s50;
  if (k1 < (-200))
  {
    k1 = -200;
  }
  else
    if (k1 > 100)
  {
    k1 = 100;
  }
  s4 = (s4 * (k1 + 1000)) / 1000;
  n3 = q->s88 + (s4 - s1);
  q->s88 = n3;
  if (n3 < 0)
  {
    q->s88 = 0;
  }
  else
    if (n3 >= 0x3A99)
  {
    q->s88 = 0x3A98;
  }
  lo = q->s88 * s3;
  q->sA0 = lo;
  if (a0->s1B8 != 0)
  {
    a0->s1E0 = 0;
    t = a0->s1B8 | 0x100;
    a0->s1B8 = t;
    a0->s1D8 = (a0->s1D8 * 999) / 1000;
    if (t == 0x100)
    {
      q->s3C = 30;
      q->s40 = (q->s88 - ((a0->s1D8 * 10000) / ((s->s2 * 0x490) / 160))) / 30;
      a0->s1B8 = 0;
    }
  }
  else
    if (((unsigned) (q->sA4 - 1)) < 2)
  {
    n2 = 0x1000 - func_800904F4(func_80025F84(a0->s1D4, a0->s54));
    x = (n2 * 100) >> 12;
    k3 = r->sE;
    k3 = k3 + 9000;
    a0->s1D8 = (a0->s1D8 * (k3 - x)) / 10000;
    a0->s1E0 = 0;
  }
  else
    if (q->s3C > 0)
  {
    x = lo;
    if (x < 0)
    {
      x += 0x1FFFF;
    }
    a0->s1E0 = ((x >> 17) * (30 - q->s3C)) / 30;
    k2 = (((a0->s1D8 * 6) / 100) * (30 - q->s3C)) / 30;
    a0->s1D8 = a0->s1D8 - k2;
  }
  else
  {
    n5 = lo;
    if (n5 < 0)
    {
      n5 += 0x1FFFF;
    }
    a0->s1E0 = n5 >> 17;
    k2 = (a0->s1D8 * 6) / 100;
    a0->s1D8 = a0->s1D8 - k2;
  }
  if (a0->s1D8 < 8)
  {
    a0->s1D4 = a0->s54;
  }
  if (D_800FF6B0 >= 2)
  {
    switch (q->sA4)
    {
      case 0:
        func_80024CD8(a0);
        break;

      case 2:
        func_800252EC(a0);
        break;

      case 4:
        func_80025954(a0);
        break;

    }

  }
  else
  {
    a0->s1D8 = 0;
  }
  if (q->sAC == 0)
  {
    if (a0->s1D8 < 0x20)
    {
      a0->s1D8 = a0->s1D8 >> 1;
    }
  }
  if ((((char *) a0) == D_800ABEF8) || (((char *) a0) == (D_800ABEF8 + 0x320)))
  {
    d1 = D_800F4C70[q->s24 & 1];
    k2 = a0->s1DE;
    if (k2 > 0)
    {
      k2 = 0x64;
    }
    else
    {
      k2 += 0x50;
    }
    if (k2 <= 0)
    {
      k2 = 1;
    }
    D_800F4C70[q->s24 & 1] = d1 + (((q->s88 - d1) * 0x14) / k2);
    if (D_800F4C70[q->s24 & 1] < 0x1F5)
    {
      D_800F4C70[q->s24 & 1] += 0x1F4 - ((func_80096464() * 0x64) / 32767);
    }
    D_800F2AB4[q->s24 & 1] = (q->sAC >= 0x41) && (q->s38 == 0);
  }
}
