
extern int func_8003069C();
extern int func_80020074();
extern unsigned char D_8009F074[][4];
extern signed char D_8009F108[][8];
extern unsigned char D_8009F1E0[];
extern unsigned char D_8009F1E4[];
extern int D_800ACC14;
extern struct 
{
  char pad[0x17];
  unsigned char v;
} D_800AD4D8;
int func_80030D9C(int x, int a1)
{
  int s3 = 0x78;
  unsigned char (*new_var)[4];
  int i = 0;
  int s0;
  int s2;
  int a;
  do
  {
    s2 = a1 - (i * 0x32);
    if (s2 >= 0x81)
    {
      s2 = 0x80;
    }
    else
      if (s2 < 0)
    {
      s2 = 0;
    }
    s0 = ((signed char) D_800AD4D8.v) >> 1;
    if (s0 >= 3)
    {
      s0 = 2;
    }
    if (i == 2)
    {
      s0 += 0xB;
      a = s0;
    }
    else
    {
      a = s0;
      if (i == 1)
      {
        s0 = ((signed char) D_800AD4D8.v) + 3;
        a = s0;
      }
    }
    s3 += D_8009F1E0[i];
    x = func_8003069C(a, D_800ACC14 + 0x70, x, 0x20, s3, s2);
    if (i == 1)
    {
      unsigned char *t;
      int b = D_8009F108[s0][4];
      s0 = D_8009F1E4[(signed char) D_800AD4D8.v] + 0x20;
      new_var = D_8009F074;
      t = new_var[s0];
      x = func_80020074(D_800ACC14 + 0x70, x, b + 0x34, s3, t[2], t[3], t[0], t[1], 0x4A, 0x2E28, i, s2);
    }
    i++;
  }
  while (i < 3);
  return x;
}
