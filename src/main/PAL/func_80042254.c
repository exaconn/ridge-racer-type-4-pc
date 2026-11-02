
/* RODATA 0x80010ecc 0x34 */
typedef unsigned char u8;
extern int func_8001F5B4();
extern int func_8001FCC8();
extern int func_8001FD20();
extern int func_8001FF84();
extern int func_8002B75C();
extern int func_8002B814();
typedef struct R4
{
  u8 a;
  u8 b;
  u8 c;
  u8 d;
} R4;
extern R4 D_8009FA1C[];
extern char D_8009FDBF[];
extern char D_8009FDC0[];
extern char D_8009FDC4[];
extern u8 D_8009FDCC[];
extern u8 D_8009FDD8[];
extern u8 D_8009FDE4[];
extern int D_800AC5D4;
extern signed char *D_800F2AB0;
extern char *D_800F39C8[];
extern char *D_800F3AFC;
extern R4 D_800F4D98[];
int func_80042254(arg0, arg1, arg2)
int arg0;
int arg1;
int arg2;
{
  char sp28[8];
  int sp30;
  int sp34;
  int sp38;
  int sp3C;
  int sp40;
  int sp44;
  int sp48;
  char *sp4C;
  char *a1p;
  int x;
  int y;
  int tx;
  int i;
  int s1;
  int s2;
  int s6;
  int j2;
  int v;
  int t3;
  int t4;
  char *rec;
  u8 *e;
  signed char *f;
  R4 *ent;
  char *c;
  char *q;
  char *q8;
  x = *((short *) ((D_800F3AFC + (arg2 * 0x1C)) + 4));
  y = *((short *) ((D_800F3AFC + (arg2 * 0x1C)) + 6));
  sp30 = *((short *) ((D_800F3AFC + (arg2 * 0x1C)) + 0xC));
  sp3C = (D_800F4D98[D_800AC5D4].b * 2) + 8;
  sp38 = D_800F4D98[D_800AC5D4].a * 2;
  sp34 = (D_800F4D98[D_800AC5D4].c ^ 0x27) == 0;
  if ((D_800F4D98[D_800AC5D4].c ^ 0x27) == 0)
  {
    if (D_800F4D98[D_800AC5D4].a == 2)
    {
      sp4C = D_8009FDC4;
    }
    else
    {
      if (D_800F4D98[D_800AC5D4].a == 0)
      {
        func_8002B75C(D_8009FDBF, "D ");
      }
      else
      {
        func_8002B75C(D_8009FDBF, "G ");
      }
      q = D_8009FDC0;
      func_8002B814(q, D_800F4D98[D_800AC5D4].b + 1, 0);
      sp4C = q - 8;
    }
  }
  c = D_800F39C8[D_800AC5D4];
  sp48 = 0x11;
  sp44 = ((u8) c[0x1F3]) + 1;
  sp40 = *((short *) (c + 0x1F0));
  if ((*((short *) (c + 0x1D2))) > 0)
  {
    sp48 = 0x12;
  }
  i = 7;
  q8 = &sp28[7];
  do
  {
    *q8 = 0;
    i--;
    q8--;
  }
  while (i >= 0);
  y += 0x1E;
  for (i = 0; i < 0xC; i++)
  {
    x += D_8009FDCC[i];
    e = D_8009FDD8 + i;
    arg1 = func_8001FF84(arg0, arg1, x, y, &D_8009FA1C[*e], 0x7FF8, 0);
    t3 = x + 2;
    s2 = t3 + D_8009FA1C[*e].c;
    if (D_800F2AB0[1] != 6)
    {
      switch (i)
      {
        case 1:

        case 7:
          if (sp34 == 0)
        {
          if (i == 1)
          {
            s6 = sp3C;
          }
          else if (i == 7)
          {
            s6 = sp38;
          }
          for (s1 = s6; s1 < (s6 + 2); s1++)
          {
            f = ((signed char *) D_8009FDE4) + s1;
            arg1 = func_8001FF84(arg0, arg1, s2, y, &D_8009FA1C[*f], 0x7FF8, 0);
            t4 = s2 + 2;
            s2 = t4 + D_8009FA1C[*f].c;
          }
        }
        else
        {
          arg1 = func_8001FD20(arg0, arg1, s2, y + 8, 0x38, 2, 0xFF, 0xFF, 0xFF);
        }
          break;

        case 3:
          if (sp34 == 0)
        {
          arg1 = func_8001F5B4(arg0, arg1, s2, y, D_800F39C8[D_800AC5D4] + 0x1F5, 0x7FF8, 2, 0);
        }
        else
        {
          arg1 = func_8001F5B4(arg0, arg1, s2, y, sp4C, 0x7FF8, 2, 0);
        }
          break;

        case 5:

        case 8:
          s6 = 2;
          if (i == 5)
        {
          s1 = sp44;
          s6 = 1;
        }
        else
        {
          s1 = sp40;
        }
          func_8002B814(sp28, s1, s6);
          arg1 = func_8001F5B4(arg0, arg1, s2, y, sp28, 0x7FF8, 0, 0);
          break;

        case 11:
          arg1 = func_8001FF84(arg0, arg1, s2, y, &D_8009FA1C[sp48], 0x7FF8, 0);
          break;

      }

    }
    if (i == 5)
    {
      y += 0x1C;
      x = *((short *) ((D_800F3AFC + (arg2 * 0x1C)) + 4));
    }
  }

  arg1 = func_8001FCC8(arg0, arg1, 0x38);
  y = y - 8;
  x = *((short *) ((D_800F3AFC + (arg2 * 0x1C)) + 4));
  arg1 = func_8001FD20(arg0, arg1, x, y, sp30, 2, 0xFF, 0xFF, 0xFF);
  for (i = 0; i < 4; i++)
  {
    v = -0x18;
    if (!(i < 2))
    {
      v = 4;
    }
    sp30 = 0xE4;
    if (i & 1)
    {
      sp30 = 0x78;
    }
    arg1 = func_8001FD20(arg0, arg1, x + sp30, y + v, 1, 0x14, 0xFF, 0xFF, 0xFF);
  }

  return arg1;
}
