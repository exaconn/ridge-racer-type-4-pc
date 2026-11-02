typedef struct { char p0; char b1; char pad[0xE]; int w10; char pad2[8]; short h1C; short h1E; char pad3[0x20]; char c40; char pad4[0x1B]; } W10_T;
extern W10_T D_800F3A38[];
extern char D_8009FFDC[];
extern char D_8009FFE4[];
extern int func_800978E0();
extern int func_800979AC();
extern int func_80097AA4();
extern int func_80097C20();
extern int func_80097CA0();
extern int func_80096434();
extern int func_8004BB84();
void func_8004BE4C(a0)
int a0;
{
  int w11_r;
  int s3 = a0;
  int s2 = a0 * 16;
  /* ASM_BARRIER: pin result var to $s0 (original assigns s0=result, s1=struct pointer) */
  register int s0 __asm__("$16");
  int v1;
  W10_T *s1 = &D_800F3A38[a0];
  s0 = func_800978E0(s2);
  if (s0 == 0)
  {
    ((char *)s1)[1] = 0;
    s1->w10 = -1;
    return;
  }
  if (func_800979AC(s2, 2, 0) == 0)
    s1->h1C = 0;
  else
    s1->h1C = 1;
  if (s0 == 1)
    ((unsigned char *)s1)[0x1B] = 0;
  if (s1->h1E != 0)
  {
    if (((unsigned char *)s1)[0x1B] != 0)
    {
      ((unsigned char *)s1)[0x1B] = 0;
      goto A;
    }
    goto B;
  }
A:
  if (((unsigned char *)s1)[0x1B] != 0)
    return;
B:
  if (s0 == 2)
  {
    s1->c40 = 0;
    ((unsigned char *)s1)[0x1B] = 1;
    s1->h1E = 0;
    return;
  }
  if (s0 != 6)
    return;
  { int t = func_800979AC(s2, 1, 0); v1 = func_800979AC(s2, 2, 0); s0 = t; }
  if (s0 == 0xE && v1 == 0x300)
  {
    func_80096434((char *)s1 + 0x40, 0x1C);
    s1->c40 = 1;
    s1->w10 = 2;
    func_80097CA0(s2, (char *)s1 + 0x44, 1);
    if (func_80097C20(s2, D_8009FFE4) != 0)
    {
      ((unsigned char *)s1)[0x1B] = 1;
      s1->h1E = 0;
      func_8004BB84(s3, 3);
    }
    return;
  }
  if (s0 != 2)
  {
    s1->c40 = 0;
    s1->w10 = 0;
    s0 = 0;
    s3 = func_80097AA4(s2, -1, s0);
    if (s3 == 1)
    {
      s0 = func_80097AA4(s2, 0, 1);
      v1 = func_80097AA4(s2, 0, 2);
      w11_r = s0;
      if (w11_r == 4 && v1 == 3)
        s1->c40 = s3;
    }
    func_80097CA0(s2, (char *)s1 + 0x19, 2);
    if (func_80097C20(s2, D_8009FFDC) == 0)
      return;
    ((unsigned char *)s1)[0x1B] = 1;
    s1->h1E = 0;
  }
  else
  {
    s1->w10 = -1;
    s1->c40 = 0;
    ((unsigned char *)s1)[0x1B] = 1;
  }
}
