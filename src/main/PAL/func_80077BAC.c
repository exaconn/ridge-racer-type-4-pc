
#include "rr_asm.h"
extern int func_80090C74();
extern int D_800FF690[];
extern char *D_8010ECB4;
void func_80077BAC(int a0, int *a1, int a2)
{
  short sp10[4];
  int sp18[4];
  int *sp = (int *) 0x1F800000;
  char *p;
  int v;
  RR_GTE_SETROT(D_800FF690);
  sp[0x78 / 4] = 0;
  sp10[0] = ((*((int *) ((((char *) (a0 * 0x2C)) + ((int) D_8010ECB4)) + 0xC))) - sp[2]) >> 2;
  sp10[1] = ((*((int *) ((((char *) (a0 * 0x2C)) + ((int) D_8010ECB4)) + 0x10))) - sp[3]) >> 2;
  sp10[2] = ((*((int *) ((((char *) (a0 * 0x2C)) + ((int) D_8010ECB4)) + 0x14))) - sp[4]) >> 2;
  func_80090C74(sp10, sp18);
  if (a2 == 2)
  {
    v = -sp18[0];
  }
  else
  {
    v = sp18[0];
  }
  a1[5] = (v << 1) << 1;
  a1[6] = sp18[1] << 2;
  a1[7] = sp18[2] << 2;
  RR_GTE_SETTR_AT20(a1);
  RR_GTE_SETROT(a1);
}
