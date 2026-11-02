
extern short D_800F4D44;
extern struct 
{
  char pad0[0x2C];
  short h2C;
  char pad2E[0x19D0 - 0x2E];
  int f19D0;
  int f19D4;
  int f19D8;
  int f19DC;
  int f19E0;
  unsigned short h19E4;
  short h19E6;
  short arr[1];
  char pad19EA[0x1A28 - 0x19EA];
  int f1A28;
} D_800F4DB8;
extern struct 
{
  char pad[0x31];
  unsigned char b31;
  short h32;
} D_800AD528;
extern int func_8007EA34();
extern int func_800857A4();
void func_8004F418()
{
  int s0 = 0;
  unsigned short s1;
  int v1;
  register int v0 asm("$2"); /* ASM_BARRIER: pin v0 so both copies read the sra result */
  int t0;
  int t1;
  unsigned short s2;
  if (D_800F4D44 == 1)
  {
    if (D_800F4DB8.f1A28 > 0)
    {
      D_800F4DB8.f1A28 = D_800F4DB8.f1A28 - 1;
    }
  }
  if (D_800F4DB8.f19D0 != 0)
  {
    s2 = D_800F4DB8.arr[D_800F4DB8.f19D8];
    if (D_800F4DB8.arr[D_800F4DB8.f19D8] >= 0)
    {
      if (D_800F4DB8.f19E0 != 0)
      {
        if (D_800F4DB8.f19D8 > 0)
        {
          goto b7;
        }
      }
      else
      {
        b7:
        if (func_8007EA34(0x10, D_800F4DB8.f19D8) == 1)
        {
          s0 = -1;
        }

      }
      if (s0 == 0)
      {
        v1 = D_800F4DB8.h19E6 * D_800AD528.b31;
        s1 = D_800F4DB8.h19E4;
        if (v1 < 0)
        {
          v1 += 0xF;
        }
        t1 = v1 >> 4;
        v0 = ((short) s1) * D_800AD528.b31;
        s0 = t1;
        if (v0 < 0)
        {
          v0 += 0xF;
        }
        t0 = v0 >> 4;
        s1 = t0;
        if (D_800AD528.h32 != 0)
        {
          v0 = (((short) t0) + ((short) t1)) / 2;
          s0 = v0;
          s1 = v0;
        }
        func_800857A4(4, D_800F4DB8.h2C, (short) s2, 0, 0x3C, 0, (short) s1, (short) s0);
        func_800857A4(5, D_800F4DB8.h2C, (short) s2, t1 = 1, 0x3C, 0, (short) s1, (short) s0);
        D_800F4DB8.f19D8 = D_800F4DB8.f19D8 + t1;
      }
    }
    else
    {
      D_800F4DB8.f19D0 = 0;
      D_800F4DB8.f19D8 = 0;
      D_800F4DB8.f19DC = 0;
    }
  }
}
