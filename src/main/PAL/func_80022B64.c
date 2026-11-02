
extern int D_800FF6B0;
extern int D_800FF6F8;
extern int D_800AD504;
extern int D_800AC538;
extern short D_800AD508;
extern char D_800ABEF8[];
extern short func_80022848(void);
extern void func_80026800();
extern void func_80022D50();
extern void func_8002367C();
extern void func_800394B8();
extern void func_800394E4();
extern void func_800512C4();
extern void func_800172DC();
extern int func_8004C370();
void func_80022B64(char *a0, int a1)
{
  int pad[18];
  int *pp = pad;
  if (D_800FF6B0 > 0)
  {
    *((short *) (a0 + 0x1EA)) = func_80022848();
    func_80026800(a0, a1);
    func_80022D50(a0, a1);
    func_8002367C(a0);
    if ((a0 == D_800ABEF8) || (a0 == (D_800ABEF8 + 0x320)))
    {
      if (D_800FF6F8 == 1)
      {
        func_800394B8(a0);
      }
      if (D_800AD504 != 2)
      {
        func_800394E4(a0, a1 & 1);
      }
    }
  }
  else
    if (D_800FF6B0 == 0)
  {
    func_800512C4(a1 & 1, 0, 0);
  }
  if (D_800AD508 < 0)
  {
    if (func_8004C370(a1 & 1) != 0)
    {
      if (((unsigned) (D_800FF6B0 - 2)) < 2)
      {
        unsigned short t = (*((unsigned short *) (a0 + 0x1E4))) ^ 0x100;
        D_800AC538 ^= 1;
        *((unsigned short *) (a0 + 0x1E4)) = t;
 do { func_800172DC(t); } while (0);
      }
    }
  }
}
