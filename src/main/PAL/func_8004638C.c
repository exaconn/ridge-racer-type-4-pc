
typedef struct 
{
  short x;
  short y;
} V;
typedef struct 
{
  short x;
  short y;
  int z;
} P;
extern int D_800ABECC;
extern P *D_800ACB9C;
extern void func_800464E8();
void func_8004638C(a0)
char *a0;
{
  struct 
  {
    V a;
    V b;
    int x0;
    int p0;
    int y0;
    int p1;
    int x1;
    int p2;
    int y1;
    int p3;
    int o1;
    int o2;
  } w;
  int d = 0x400 - (*((short *) (a0 + 0x170)));
  int i = d;
  int j;
  int k;
  int m;
  P *new_var;
  P *t;
  char *s = a0 + 0x1F4;
  if (i < 0)
  {
    i += 0x3FF;
  }
  j = (*((int *) (a0 + 0xF0))) - (i >> 10);
  if (j < 0)
  {
    j += D_800ABECC;
  }
  k = j - 1;
  if (k < 0)
  {
    k += D_800ABECC;
  }
  new_var = D_800ACB9C + j;
  t = new_var;
  w.x0 = t->x;
  w.y0 = t->y;
  t = D_800ACB9C + k;
  w.x1 = t->x;
  w.y1 = t->y;
  m = d & 0x3FF;
  {
    int u = (w.x1 - w.x0) * m;
    int v;
    if (u < 0)
    {
      u += 0x3FF;
    }
    v = (w.y1 - w.y0) * m;
    w.a.x = w.x0 + (u >> 10);
    if (v < 0)
    {
      v += 0x3FF;
    }
    w.b.x = w.y0 + (v >> 10);
  }
  func_800464E8(w.a, w.b, *((int *) (a0 + 0xF0)), &w.o1, a0 + 0x232, &w.o2);
  if ((*((short *) (s + 0x38))) == 0)
  {
    *((short *) (s + 0x3C)) = *((short *) (s + 0x3E));
  }
}
