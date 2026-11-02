/* RODATA 0x800106d0 0x1c */

extern int func_8001FCC8();
extern int func_8001FF84();
extern unsigned char D_8009F730[];
extern unsigned char D_8009F790[];
extern unsigned short D_800AC648;
extern int D_800ACC14;
extern signed char D_800AD4EF;
extern short D_800F4D44;
void func_8003662C(a0)
int a0;
{
  int s4;
  int a1;
  int sz2;
  int new_var;
  int s3;
  int s1;
  int s2;
  int s0;
  int t0;
  int d;
  /* ASM_BARRIER: pin t to $v1 like the original */
  register int t asm("$3");
  int u;
  unsigned char *p;
  unsigned char *q;
  unsigned char *base;
  s4 = 0x80;
  if (a0 < 0x41)
  {
    s4 = a0 * 2;
  }
  a1 = *((int *) 0x1F800000);
  s3 = 0x50;
  s1 = 0;
  do
  {
    if (s1 < 2)
    {
      d = D_800F4D44;
      if ((d != 1) && (s1 == 1))
      {
        goto next;
      }
      switch (d)
      {
        case 1:
          s2 = D_8009F790[(D_800AD4EF * 2) + s1];
          break;

        case 0:
          s2 = 3;
          break;

        case 2:

        case 3:
          s2 = 0;
          break;

        case 4:
          s2 = 1;
          break;

        case 6:
          s2 = 2;
          break;

      }

    }
    if (s1 == 2)
    {
      s2 = 0xC;
    }
    else
      if (s1 == 3)
    {
      t = s2 + 1;
      s2 = t + (D_800AC648 & 7);
    }
    p = D_8009F730 + (s2 * 4);
    a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s3, 0x48, p, 0x263A, (s4 + 1) * 2);
    u = s3 + 2;
    s3 = u + p[2];
    next:
    s1++;

  }
  while (s1 < 4);
  s2 = 0x16;
  if (D_800F4D44 != 1)
  {
    s0 = 1;
    s3 = 0xD6;
  }
  else
  {
    s2 = 0x15;
    s0 = 2;
    s3 = 0xE6;
  }
  t0 = 0x263A;
  s1 = 0;
  if (s0 != 0)
  {
    base = D_8009F730;
    do
    {
      new_var = s3 - 2;
      a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s3, 0x24, (unsigned char *)((s2 * 4) + (int)base), t0, (s4 + 1) * 2);
      s2 = 0x17;
      s1++;
      q = base + 0x5C;
      t = new_var;
      s3 = t - q[2];
      t0 = 0x263B;
    }
    while (s1 < s0);
  }
  *((int *) 0x1F800000) = func_8001FCC8(D_800ACC14 + 0x70, a1, 0x2E);
}
