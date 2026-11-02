extern int func_8002B814();
void func_8002B934(char *a, int b)
{
  int s;
  int t;
  int m;
  int r;
  if (b >= 0)
  {
    if (s)
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      func_8002B814(a, m, 1);
      func_8002B814(a + 3, r, 1);
      func_8002B814(a + 6, t, 2);
      return;
    }
    else
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      func_8002B814(a, m, 1);
      func_8002B814(a + 3, r, 1);
      func_8002B814(a + 6, t, 2);
      return;
    }
  }
  a[8] = 0x2D; a[7] = 0x2D; a[6] = 0x2D; a[4] = 0x2D; a[3] = 0x2D; a[1] = 0x2D; a[0] = 0x2D;
}
