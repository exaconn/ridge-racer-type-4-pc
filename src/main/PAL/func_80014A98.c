
extern int func_80014068();
extern int func_8001446C();
extern int func_8001E84C();
extern int func_8006EE3C();
extern int D_800100B8;
extern int D_8009E0AC;
int func_80014A98(int a0, int a1, int a2, int a3)
{
  int *s0 = (int *) 0x1F80007C;
  int *p;
  int *g;
  int w;
  unsigned new_var3;
  unsigned new_var2;
  int t;
  /* ASM_BARRIER: pin a to $5 to match original register allocation */
  register unsigned a __asm__("$5");
  unsigned long new_var;
  unsigned b;
  if (D_8009E0AC < 0)
  {
    return 0;
  }
  func_8006EE3C(0x1F80007C, a0, a1);
  new_var = 3;
  s0[2] = a3;
  a = D_8009E0AC & 0xF;
  a <<= 6;
  b = D_8009E0AC & 0x10;
  b <<= 4;
  s0[3] = (((b + 0x7F) << 6) | ((a >> 4) | new_var)) << 16;
  new_var3 = b >> 4;
  new_var2 = (a >> 6) | 0x20;
  s0[4] = (new_var3 | new_var2) << 16;
  g = (int *) 0x1F800000;
  s0[0] = g[0x54 / 4];
  s0[1] = g[0x58 / 4];
  s0[5] = g[1] + 0x200;
  p = *((int **) ((a2 * 4) + g[0x50 / 4]));
  while ((w = *(p++)) != 0)
  {
    t = w >> 16;
    w &= 0xFFFF;
    switch (w)
    {
      case 0:
        p = (int *) (((char *) p) + (t * 0x10));
        break;

      case 1:
        p = (int *) (((char *) p) + (t * 0x18));
        break;

      case 2:
        p = (int *) func_80014068(p, s0, t);
        break;

      case 3:
        asm volatile("mtc2 %0, $6" : : "r"(0x3C808080));
        p = (int *) func_8001446C(p, s0, t);
        break;

      default:
        return func_8001E84C(&D_800100B8, w, t);

    }

  }

  return 1;
}
