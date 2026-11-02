
#include "rr/race.h"

extern char D_800AD528[];
extern short D_8010BFD0[];
extern void func_800172DC();
extern void func_8006E030();
extern void func_8006E09C();
extern void func_8006E108();
extern int func_8006E368();
extern int func_80090424();
extern int func_800904F4();
extern int func_80090758();
extern void func_80090CA4();
extern void func_80090DB4();
extern void func_80090EC4();
void func_80033474(RrRaceVehicle *a0, int a1)
{
  int sp10[3];
  char m20[0x20];
  char m40[0x20];
  int *g = (int *) 0x1F800000;
  int s0;
  int v0;
  int v1;
  int i;
  int k;
  char **new_var;
  char *p;
  char *q;
  short *s5;
  if (a1 >= 0x4B)
  {
    return;
  }
  if (a1 < 2)
  {
    func_800172DC();
    D_8010BFD0[0] = 0;
    D_8010BFD0[2] = -0x100;
    D_8010BFD0[1] = -0x100;
    if ((a1 == 1) && (a0 == D_800ABEF8))
    {
      i = 0;
      k = 2;
      p = D_800AD528;
      q = (char *)&D_800AC650;
      do
      {
        *((int *) (q + 0xA8)) = *((signed char *) (p + 0x2A));
        *((char *) ((*(new_var = &p)) + 0x2A)) = k;
        p += 4;
        q += 4;
        i++;
      }
      while (i < 2);
    }
  }
  else
  {
    s0 = (a1 << 11) / 75;
    v0 = func_80090424(s0);
    s5 = D_8010BFD0;
    if (v0 < 0)
    {
      v0 += 7;
    }
    s5[0] = v0 >> 3;
    v0 = -(func_800904F4(s0) << 9);
    if (v0 < 0)
    {
      v0 += 0xFFF;
    }
    s5[2] = v0 >> 12;
    s5[1] = ((a1 << 8) / 75) - 0x200;
  }
  func_8006E09C(m20, 0x800 - a0->rot54);
  func_8006E108(m40, a0->rot50);
  func_80090CA4(m20, m40);
  func_8006E030(m40, a0->rot58);
  func_80090DB4(m20, m40);
  func_80090EC4(m40, D_8010BFD0, sp10);
  g[2] = a0->pos10[0] + sp10[0];
  g[3] = a0->pos10[1] + sp10[1];
  g[4] = a0->pos10[2] + sp10[2];
  v1 = func_80090424(i = a0->rot54) * 0x60;
  if (v1 < 0)
  {
    v1 += 0xFFF;
  }
  sp10[0] = (a0->pos10[0] + (v1 >> 12)) - g[2];
  sp10[1] = a0->pos10[1] - g[3];
  v1 = func_800904F4(a0->rot54) * 0x60;
  if (v1 < 0)
  {
    v1 += 0xFFF;
  }
  v1 >>= 12;
  sp10[2] = (a0->pos10[2] + v1) - g[4];
  v0 = func_8006E368(sp10[0], sp10[2]);
  g[7] = 0x400 - v0;
  v0 = func_80090758((sp10[0] * sp10[0]) + (sp10[2] * sp10[2]));
  v0 = func_8006E368(sp10[1], v0 >> 6);
  g[6] = 0x400 - v0;
  g[8] = 0;
  if (a1 < 0x49)
  {
    return;
  }
  a0->flags1E4 |= 0x100;
  if ((a1 == 0x49) && (a0 == D_800ABEF8))
  {
    i = 0;
    q = (char *)&D_800AC650;
    p = D_800AD528;
    do
    {
      *((unsigned char *) (p + 0x2A)) = *((unsigned char *) (q + 0xA8));
      q += 4;
      i++;
      p += 4;
    }
    while (i < 2);
  }
}
