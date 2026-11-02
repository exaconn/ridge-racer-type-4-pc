
extern char *D_800ACC14;
extern unsigned char D_800A0DC8[][4];
void func_80056F04(int a0, int a1, int a2, int a3, int a4)
{
  unsigned short sp[2];
  unsigned char *t0;
  unsigned char *p;
  int *g;
  register char *d asm("$5");
  register unsigned lo asm("$6");
  unsigned hi;
  unsigned cmd;
  unsigned char new_var;
  t0 = D_800A0DC8[a0];
  if (a4 != 0)
  {
    sp[0] = 0xA0 - (t0[2] >> 1);
    sp[1] = a2;
  }
  else
  {
    sp[0] = a1;
    sp[1] = a2;
  }
  g = (int *) 0x1F800000;
  p = *((unsigned char **) g);
  p[3] = 4;
  p[7] = 0x66;
  p[4] = a3;
  p[5] = a3;
  p[6] = a3;
  *((short *) (p + 8)) = sp[0];
  *((short *) (p + 0xA)) = sp[1];
  *((short *) (p + 0x10)) = t0[2];
  *((short *) (p + 0x12)) = t0[3];
  p[0xC] = t0[0];
  hi = 0xFF000000;
  new_var = t0[1];
  *((short *) (p + 0xE)) = 0x7F38;
  p[0xD] = new_var;
  lo = 0xFFFFFF;
  cmd = 0xE1000229;
  d = D_800ACC14;
  *((unsigned int *) p) = ((*((unsigned int *) p)) & hi) | ((*((unsigned int *) (d + 0x70))) & lo);
  *((unsigned int *) (d + 0x70)) = ((*((unsigned int *) (d + 0x70))) & hi) | (((unsigned int) p) & lo);
  p += 0x14;
  *g = (int) p;
  p[3] = 1;
  d = D_800ACC14;
  *((unsigned int *) (p + 4)) = cmd;
  *((unsigned int *) p) = ((*((unsigned int *) p)) & hi) | ((*((unsigned int *) (d + 0x70))) & lo);
  *((unsigned int *) (d + 0x70)) = ((*((unsigned int *) (d + 0x70))) & hi) | (((unsigned int) p) & lo);
  p += 8;
  *g = (int) p;
}
