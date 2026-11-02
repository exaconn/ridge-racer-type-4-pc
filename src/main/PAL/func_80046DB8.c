
#include "types.h"
#include "rr/race.h"

extern int func_800469A4();
extern int func_80046B1C();
extern int func_8004638C();
extern RrRaceStandingsTable *D_800AC738;
extern int D_800AC664;
extern short D_800AC59C;

void func_80046DB8(RrRaceVehicle *p)
{
  RrRaceVehicleControl *q;
  RrRaceStandingsTable *s5;
  RrRaceVehicle *s1;
  int x8;
  int u1;
  int u2;
  int t8;
  int ab;
  int w1;
  int cnt;
  int flg;
  int n;
  int ab2;
  int th2;
  int c0;
  int e1;
  int e2;
  int e3;
  int s2;
  int s4;
  int s6;
  int a0;
  int v0;
  int v1;
  int k;
  int tt;
  s6 = 0;
  s4 = -1;
  s5 = D_800AC738;
  q = (RrRaceVehicleControl *)((char *)p + 0x1F4);
  s2 = func_800469A4(p);
  if (s2 >= 0)
  {
    k = s2 * 12;
    s1 = ((RrRaceStandingsTable *)((char *)s5 + k))->entries18[0].vehicle08;
    if ((func_80046B1C(p, s2) > 0) && (q->boostTimer44 == 0))
    {
      if (q->laneActive6A == 0)
      {
        e1 = (u16)q->laneTarget3C;
        q->laneTimer38 = 10;
        q->laneActive6A = 1;
        q->laneMode88 = 0;
        q->laneStart3A = e1;
        ab = s5->gap14;
        if (ab < 0)
        {
          ab = -ab;
        }
        e3 = (short)((u16)p->halfSpan10E);
        e3 = e3 + (e3 / 2);
        if (ab < e3)
        {
          e1 = s1->lateralOffset172;
          if (e1 < p->lateralOffset172)
          {
            e2 = (e1 + 0x400) / 2;
          }
          else
          {
            tt = e1 - 0x400; tt = tt + ((unsigned)tt >> 31);
            __asm__ volatile("" : "=r"(tt) : "0"(tt)); /* ASM_BARRIER: keep the two /2 arms from being cross-jumped */
            e2 = tt >> 1;
          }
        }
        else
        {
          e2 = s1->lateralOffset172;
          e1 = (u16)s1->lateralOffset172;
          if (e2 > 0)
          {
            e2 = e1 - 0x200;
          }
          else
          {
            e2 = e1 + 0x200;
          }
        }
        *(volatile s16 *)&q->laneTarget3C = e2;
        if (q->laneTarget3C < (-0x220))
        {
          q->laneTarget3C = -0x220;
        }
        if (q->laneTarget3C > 0x220)
        {
          q->laneTarget3C = 0x220;
        }
        q->laneDelta6C = ((u16)q->laneTarget3C) - ((u16)q->laneStart3A);
      }
    }
    else
    {
      s2 = -1;
    }
  }
  if (D_800AC664 > 0)
  {
    x8 = s5->dist08;
    if ((x8 < 0) && (q->metric12A < x8))
    {
      s4 = 0;
    }
    if (D_800AC664 == 2)
    {
      if ((s5->dist08 < s5->dist0C) && (q->metric12A < s5->dist0C))
      {
        s4 = 1;
      }
    }
    if (s4 >= 0)
    {
      if ((q->laneActive6A == 0) && (q->laneMode88 == 0))
      {
        s1 = &D_800ABEF8[s4];
        w1 = q->relDist74 + q->relDist78;
        if (w1 < 0)
        {
          if (s1->lateralOffset172 < p->lateralOffset172)
          {
            cnt = 0;
            u1 = (u16)q->laneTarget3C;
            q->laneTimer38 = 10;
            q->laneActive6A = 1;
            q->laneMode88 = 2;
            q->laneStart3A = u1;
          }
        }
        else
        {
          t8 = s1->lateralOffset172;
          if (p->lateralOffset172 < t8)
          {
            u2 = (u16)q->laneTarget3C;
            q->laneTimer38 = 10;
            q->laneActive6A = 1;
            q->laneMode88 = 1;
            q->laneStart3A = u2;
          }
        }
        s2 = 0;
      }
    }
    else
    {
      q->laneMode88 = 0;
    }
  }
  else
  {
    q->laneMode88 = 0;
  }
  if (s2 < 0)
  {
    cnt = 0;
    n = D_800AC59C;
    flg = cnt;
    if (n > 0)
    {
      do
      {
        s1 = s5->entries18[cnt].vehicle08;
        if (s1 != p)
        {
          ab2 = s5->entries18[cnt].totalDist04 - (p->first + p->second);
          if (ab2 < 0)
          {
            ab2 = -ab2;
          }
          th2 = (short)((u16)p->halfSpan10E);
          th2 = th2 + (th2 / 2);
          if (ab2 < th2)
          {
            c0 = q->laneCurrent3E;
            if (c0 < q->laneStart3A)
            {
              if (s1->lateralOffset172 > (c0 - 0x100))
              {
                s6 = 1;
              }
            }
            else
            {
              if (s1->lateralOffset172 < (c0 + 0x100))
              {
                s6 = 1;
              }
            }
          }
        }
        cnt++;
      }
      while (cnt < D_800AC59C);
    }
    switch (q->laneMode88)
    {
      case 1:
        if (q->laneStart3A < q->laneCurrent3E)
      {
        flg = 1;
      }
        break;

      case 2:
        if (q->laneCurrent3E < q->laneStart3A)
      {
        flg = 1;
      }
        break;

    }

    if (((s6 == 0) && (q->laneTimer38 != 0)) && (flg == 0))
    {
      v0 = q->laneCooldown40;
      if (v0 > 0)
      {
        q->laneCooldown40 = v0 - 1;
      }
      else
      {
        q->laneTimer38 = 0;
        q->laneActive6A = 1;
        func_8004638C(p);
        v1 = (u16)q->laneTarget3C;
        a0 = (u16)q->laneStart3A;
        q->laneMode88 = 3;
        q->laneDelta6C = v1 - a0;
      }
    }
  }
}
