
#include "rr/race.h"

struct E
{
  char pad[0x1C];
  short w;
  char pad2[0x3C - 0x1E];
};
extern short D_800AD522;
extern int D_800ABECC;
extern struct E *D_800ABEA4;
extern int func_800261BC();
void func_8002648C(RrRaceVehicle *p)
{
  int s = 1;
  int t;
  int cur;
  int back;
  int fwd;
  int k;
  int j;
  int x;
  struct E *new_var;
  int n;
  struct E *base;
  int bk;
  t = func_800261BC(p->pos20, p->trackNodeF0);
  if (t < 0)
  {
    *(short *)&p->flags1E4 = -1;
    return;
  }
  if (D_800AD522 == 0)
  {
    if (t != p->trackNodeF0)
    {
      cur = p->trackNodeF0;
      n = D_800ABECC;
      base = D_800ABEA4;
      while (1)
      {
        x = cur - s;
        back = x;
        new_var = base;
        if (x < 0)
        {
          back = n + x;
        }
        fwd = (cur + s) % n;
        if (t == back)
        {
          for (k = 0; k < s; k++)
          {
            x = p->trackNodeF0 - k;
            j = x;
            if (x < 0)
            {
              j = n + x;
            }
            p->first += new_var[j].w;
          }

          break;
        }
        if (t == fwd)
        {
          for (k = 1; s >= k; k++)
          {
            p->first -= new_var[(p->trackNodeF0 + k) % n].w;
          }

          break;
        }
        s++;
        if (t == cur)
        {
          break;
        }
      }

    }
  }
  else
    if (t != p->trackNodeF0)
  {
    cur = p->trackNodeF0;
    n = D_800ABECC;
    base = D_800ABEA4;
    while (1)
    {
      x = cur - s;
      back = x;
      if (x < 0)
      {
        back = n + x;
      }
      fwd = (cur + s) % n;
      if (t == fwd)
      {
        for (k = 0; k < s; k++)
        {
          p->first += base[(p->trackNodeF0 + k) % n].w;
        }

        break;
      }
      /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
      __asm__("" : "=r"(bk) : "0"(back));
      if (t == bk)
      {
        for (k = 1; s >= k; k++)
        {
          x = p->trackNodeF0 - k;
          j = x;
          if (x < 0)
          {
            j = n + x;
          }
          p->first -= base[j].w;
        }

        if (n)
        {
          break;
        }
        else
        {
          break;
        }
      }
      s++;
      x = p->trackNodeF0 - k;
      if (t == cur)
      {
        break;
      }
    }

  }
  p->trackNodeF0 = t;
}
