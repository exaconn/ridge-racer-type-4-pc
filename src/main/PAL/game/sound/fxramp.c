#include "types.h"

struct S { char pad[0x2874]; u32 a; char p[8]; s32 b; };

extern struct {
    char pad[0x288A];
    short h288A;
    unsigned short h288C;
    short h288E;
    short h2890;
} D_800F4DB8;
extern int func_80053A9C();
extern int func_80053CB0();

void func_80053D24();
void func_80053DE8(int a0, int a1, s16 a2);

void func_80053D24()
{
  int d = D_800F4DB8.h288A;
  int t;
  int v;
  int from;
  unsigned c;
  int to;
  if (d > 0)
  {
    c = D_800F4DB8.h288C;
    if (((short) D_800F4DB8.h288C) < d)
    {
      t = c + 1;
      to = D_800F4DB8.h288E;
      from = D_800F4DB8.h2890;
      v = (to - from) << 12;
      v = (v / d) * ((short) t);
      D_800F4DB8.h288C = t;
      if (v < 0)
      {
        v += 0xFFF;
      }
      v = (v >> 12) + from;
      func_80053A9C((short)v, &D_800F4DB8, from);
      return;
    }
    func_80053CB0(d, &D_800F4DB8);
  }
}

void func_80053DE8(int a0, int a1, s16 a2) {
    struct S *p = &D_800F4DB8;
    int v = a2;
    p->b = v;
    if ((u32)v < 100) {
        p->a++;
        if (p->a > 0x61A7F) p->a = 0;
    }
}
