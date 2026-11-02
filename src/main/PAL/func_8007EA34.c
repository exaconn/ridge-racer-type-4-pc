/* GCC_VERSION: 2.7.2 */

extern char *D_800A47D8;
extern int D_800A4844;
int func_8007EA34(a0)
int a0;
{
  int idx = -1;
  int i = 0;
  int one = 1;
  int m;
  int r;
  int x;
  int b;
  int f;
  int off;
  unsigned short h;
  loop:
  m = one << i;

  if (a0 & m)
  {
    goto found;
  }
  i++;
  if (i < 0x18)
  {
    goto loop;
  }
  check:
  if (idx != (-1))
  {
    goto have;
  }

  r = -1;
  goto out;
  found:
  idx = i;

  goto check;
  have:
  a0 = idx << 4;

  a0 = a0 + ((int) D_800A47D8);
  f = D_800A4844;
  f = f & (1 << idx);
 do { a0 = *((unsigned short *) (a0 + 0xC)); x = a0 != 0; if (f == 0) { r = x << 1; goto out; } } while (0);
  r = 1;
  if (a0 != 0)
  {
    goto out;
  }
  r = 3;
  out:
  return r;

}
