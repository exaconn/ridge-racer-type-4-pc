
extern int func_80078E40();
extern int func_8007B654();
extern int func_8007D1B8();
extern struct 
{
  unsigned short a;
  signed char b;
  unsigned char c;
  unsigned char d;
  unsigned char e;
  short f;
} D_800A4258[];
extern void (*D_800A4548[])();
extern short D_800F3A1C;
extern unsigned char D_800F3A24[];
extern unsigned short D_800F3AF8;
extern short D_800F4D3C;
extern short D_800F4D8A;
extern short D_800FFC00;
extern unsigned short D_800FFC18;
extern short D_80109478[];
extern short D_8010948E;
static inline int w4c7_nz(int arg0)
{
  return arg0 != 0;
}

void func_8007B460()
{
  int a;
  int t0;
  unsigned short v;
  int r;
  int s7;
  int s0;
  int s1;
  int s3;
  int new_var;
  int fp;
  r = 1;
  s0 = D_800A4258[D_800F3A1C].e;
  s1 = D_800A4258[D_800F3A1C].c;
  t0 = D_800A4258[D_800F3A1C].a;
  s7 = s0 & 1;
  s3 = D_800A4258[D_800F3A1C].b;
  fp = D_800F3A24[D_800FFC00];
  if (w4c7_nz(s7))
  {
    func_80078E40(D_800FFC00, 0, 0);
  }
  if (s0 & 2)
  {
    r = func_8007B654();
  }
  if (r == 1)
  {
    D_800A4548[s1]();
  }
  func_8007D1B8(s3, t0, s0, D_800FFC00, D_80109478[D_8010948E], D_800F3A24[D_800FFC00]);
  if (w4c7_nz(s7))
  {
    if (fp == 0x1E)
    {
      if (D_800F3A24[D_800FFC00] != fp)
      {
        D_800F4D8A = 0;
        D_800F4D3C = 0;
        v = D_800FFC18;
        D_800F3AF8 = v;
        a = (short) v;
        if (a == 0)
        {
          D_800F3A1C = 0;
          return;
        }
        if (((unsigned short) (v - 6)) < 2)
        {
          D_800F3A1C = 0x29;
          D_800F3AF8 = 0x10;
          return;
        }
        new_var = 1;
        if ((a == new_var) || (a == 5))
        {
          D_800F3A1C = 0x20;
        }
        else
          if (a == 2)
        {
          D_800F3A1C = 0x21;
        }
        else
          if (a == 3)
        {
          D_800F3A1C = 0x17;
        }
      }
    }
  }
}
