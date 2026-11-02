
typedef struct 
{
  int n;
  char pad[0x38 - 4];
  short a;
  short b;
} T;
typedef struct 
{
  char p0[0x18];
  T *t;
  char p1[0x32 - 0x1C];
  short s32;
  short s34;
  char p2[2];
  short s38;
  char p3[0x84 - 0x3A];
  short s84;
  short s86;
  char p4[0xA4 - 0x88];
  int sA4;
} Q;
typedef struct 
{
  char pad[0x1D8];
  short s1D8;
} O;
typedef struct 
{
  char pad[3];
  unsigned char b;
} K;
extern K D_800ADAD8[];
void func_80026800(a0, a1)
O *a0;
int a1;
{
  Q *q = (Q *) (((char *) a0) + 0x1F4);
  T *t;
  q->s34 = q->s86;
  t = q->t;
  if (q->s84 != 0)
  {
    int v = D_800ADAD8[a1].b;
    if (v & 4)
    {
      if (q->s86 < t->n)
      {
        if (q->s38 == 0)
        {
          q->s86 = q->s86 + 1;
          q->s38 = 10;
          q->s32 = 1;
          return;
        }
      }
    }
    else
      if (v & 8)
    {
      if (q->s86 >= 2)
      {
        q->s86 = q->s86 - 1;
        q->s38 = 10;
      }
    }
  }
  else
  {
    int a;
    unsigned short u, u2;
    T *e1, *e2;
    if (u = q->s86, a0->s1D8 < (e1 = &t[q->s86])->b)
    {
      if (q->s86 >= 2)
      {
        q->s86 = u - 1;
        q->s38 = 10;
        goto L;
      }
    }
    if (u2 = q->s86, a0->s1D8 >= (e2 = &t[q->s86])->a)
    {
      if (q->s38 == 0)
      {
        if (q->s86 < t->n)
        {
          q->s86 = u2 + 1;
          q->s38 = 10;
          q->s32 = 1;
        }
      }
    }
    L:
    if (a0->s1D8 == 0)
    {
      if (q->s86 >= 2)
      {
        if (q->sA4 != 4)
        {
          q->s86 = 1;
          q->s38 = 0;
        }
      }
    }

  }
}
