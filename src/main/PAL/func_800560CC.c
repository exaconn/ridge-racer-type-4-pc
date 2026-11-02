
extern int func_80055FD8();
extern short D_800A0D64[];
int func_800560CC(int a0)
{
  int pad[2];
  int i;
  int r;
  int q;
  short *p;
  int v1;
  int a1;
  int a2;
  int a3;
  int d;
  int h;
  i = func_80055FD8(D_800A0D64, a0);
  if (i == 0)
  {
    return D_800A0D64[0];
  }
  p = (short *) ((i * 4) + ((int) D_800A0D64));
  if (p[1] < 0)
  {
    return p[0];
  }
  v1 = p[-1];
  a3 = p[0];
  a1 = p[1] - v1;
  a1 = a1 * 10;
  d = a0 - (v1 * 10);
  a2 = a3 - p[-2];
  h = a1 >> 1;
  if (d < h)
  {
    r = (((d * d) * a2) / 2) / (h * h);
  }
  else
  {
    d = a1 - d;
    q = (((d * d) * a2) / 2) / (h * h);
    r = a3 - q;
  }
  q = r;
  return q;
}
