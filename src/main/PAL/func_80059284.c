
extern char D_800A1158[];
void func_80059284(short *a0, int a1)
{
  unsigned short *p = (unsigned short *) (a0 + 2);
  int sum = 0;
  unsigned short v;
  char *e;
  if (a0[2] >= 0)
  {
    do
    {
      v = p[0];
      p++;
      if (((short) v) >= 0x400)
      {
        e = D_800A1158 + (((short) (v - 0x400)) << 2);
      }
      else
      {
        e = (char *) ((((short) v) * 4) + a1);
      }
      if (a1)
      {
        sum += e[2];
        sum += ((short *) p)[0];
        p++;
      }
      else
      {
        sum += e[2];
        sum += ((short *) p)[0];
        p++;
      }
    }
    while (((short *) p)[0] >= 0);
  }
  a0[0] = 0xA0 - (sum / 2);
}
