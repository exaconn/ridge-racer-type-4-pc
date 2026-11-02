
extern int func_8004F2D8();
extern int func_80055250();
extern int D_800ABEC0;
extern short D_800AD522;
extern int D_800F2EB8;
extern int D_800F3A30;
void func_800396E4(a0)
char *a0;
{
  int lim;
  int new_var;
  int t, y;
  if (!(D_800ABEC0 & 0x100))
  {
    if (D_800AD522 != 1)
    {
      new_var = D_800F2EB8;
      lim = (D_800F3A30 - 1) * new_var;
      if (((*((int *) (a0 + 0x188))) + (*((int *) (a0 + 0x184)))) >= lim)
      {
        y = *(volatile int *)(a0 + 0xF0);
        if (func_80055250(6, y, lim) == 1)
        {
          t = D_800ABEC0 & 0x100;
          D_800ABEC0 = t;
          if (t == 0)
          {
            func_8004F2D8(0x12, 0);
            D_800ABEC0 |= 0x100;
          }
        }
      }
    }
  }
}
