
typedef struct 
{
  char b[0x2C];
} W10_R;
typedef struct 
{
  char p[0x10];
  short chk;
} W10_H;
extern int func_8007CB44();
extern char *D_800ACC0C;
extern W10_R D_800F2E30[];
extern short D_800F3A1C;
extern unsigned short D_800F4D8A;
extern int D_8010EFA8;
void func_8007C944(void)
{
  int r;
  int t0;
  int t1;
  W10_R *t2;
  short a1;
  unsigned v1;
  char *a0;
  int new_var;
  char *a2;
  W10_H *a3;
  signed char *e;
  char pad[0x38];
  if (D_800F4D8A == 0)
  {
    func_8007CB44(0);
    D_8010EFA8 = 0;
    D_800F4D8A = 1;
    return;
  }
  r = func_8007CB44(1);
  t1 = 0;
  if (r > 0)
  {
    new_var = 8;
    do
    {
      a1 = 0;
      v1 = 0;
      a0 = ((char *) (t1 * 0x2C)) + ((int) D_800ACC0C);
      do
      {
        if (v1 != new_var)
        {
          a1 += *((unsigned short *) (a0 + 0x1F60));
        }
        v1++;
        a0 = a0 + 2;
      }
      while (v1 < 0x16U);
      a2 = ((char *) (t1 * 0x2C)) + ((int) D_800ACC0C);
      a3 = (W10_H *) (a2 + 0x1F60);
      if (a1 == a3->chk)
      {
        *(t2 = D_800F2E30 + t1) = *((W10_R *) a3);
      }
      else
      {
        e = ((signed char *) D_800F2E30) + (t1 * 0x2C);
        e[0x17] = -1;
      }
      t1++;
    }
    while (t1 < 3);
    D_800F3A1C = 0x18;
    D_800F4D8A = 0;
  }
  else
    if (r < 0)
  {
    for (t1 = 2; t1 >= 0; t1--)
    {
      e = ((signed char *) D_800F2E30) + (t1 * 0x2C);
      e[0x17] = -1;
    }

    D_800F3A1C = 0x18;
    D_800F4D8A = 0;
  }
}
