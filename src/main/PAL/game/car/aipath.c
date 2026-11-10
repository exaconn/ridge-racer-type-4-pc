#include "types.h"
#include "scratchpad.h"
#include "rr_asm.h"
#include "rr/race.h"
#include "rr/track_sample_types.h"
#include "game/car/carsp.h"
#include "game/car/carmove1.h"
#include "game/car/aipath.h"
#include "rr/menu_overlay_runtime.h"
#include "game/course/trkclip.h"

typedef struct 
{
  int n;
  s32 gearData[13];
  short a;
  short b;
} T;

typedef struct 
{
  u8 b0, b1, b2;
  unsigned char b;
} K;

extern K CarInputs[];

s32 FindTrackNodeUnbounded(s32 *pos, s32 startIdx) {
    SP *g = (SP *)SCRATCH_CAR_WORK;
    s32 a, n, t, x;
    s32 r;
    /* ASM_BARRIER: keep g->s150[0] load in $v0 across g->cnt increment */
    register s32 u __asm__("$2");
    TrackSample3C *p, *q;
    s32 junk[2];
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r1 __asm__("$10");
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r2 __asm__("$11");
    /* ASM_BARRIER: GTE operand temps live in $t2/$t3/$t4 in the original PsyQ-style sequence */
    register s32 r3 __asm__("$12");

    g->cnt = 0;
    g->t110[0].a = pos[0];
    g->t110[0].c = pos[2];
    g->s150[0] = startIdx;
    g->s150[1] = TrackNodeCount;
    for (;;) {
        g->w34 = (g->s150[0] + 1) % TrackNodeCount;
        a = g->w34;
        p = &TrackNodes[g->s150[0]];
        /* ASM_BARRIER: boost p reference weight so local-alloc assigns $a2 to p and $a3 to q */
        __asm__("" : "+r"(p));
        g->w78 = p->x08;
        g->w80 = p->z0C;
        q = &TrackNodes[a];
        g->h74 = g->h70 = (u16)q->x08 - (u16)g->w78;
        g->h76 = g->h72 = (u16)q->z0C - (u16)g->w80;
        g->h64 = (u16)g->t110[0].a - (u16)g->w78;
        g->h66 = (u16)g->t110[0].c - (u16)g->w80;
        g->h70 += (u16)q->rightOffsetX00;
        g->h72 += (u16)q->rightOffsetZ02;
        g->h74 += (u16)q->leftOffsetX04;
        g->h76 += (u16)q->leftOffsetZ06;
        g->h68 = (u16)p->rightOffsetX00;
        /* ASM_BARRIER: keep r3 load from g->h64 after g->h68 store */
        __asm__ ("" : : : "memory");
        g->h6a = (u16)p->rightOffsetZ02;
        g->h6c = (u16)p->leftOffsetX04;
        g->h6e = (u16)p->leftOffsetZ06;
        r3 = *(s32 *)&g->h64;
        r1 = *(s32 *)&g->h68;
        r2 = *(s32 *)&g->h6c;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r3c);
        r1 = *(s32 *)&g->h74;
        r2 = *(s32 *)&g->h70;
        r3 = *(s32 *)&g->h64;
        RR_GTE_NCLIP3(r1, r2, r3, &g->r40);
        if (g->r3c >= 0 && g->r40 >= 0) {
            break;
        }
        n = g->cnt + 1;
        u = *(u16 *)&g->s150[0];
        /* ASM_BARRIER: schedule g->cnt + 1 before sign-extending g->s150[0] */
        __asm__("" : "+r"(u) : "r"(n));
        x = (s16)u;
        g->cnt = n;
        x = (n & 1) ? x + n : x - n;
        t = g->s150[1];
        a = g->s150[0] = x;
        if (a >= 0) {
            g->s150[0] = a % t;
        } else {
            r = (t + a) % t;
            /* ASM_BARRIER: keep modulo result in $v0 before storing to g->s150[0] */
            __asm__("" : "+r"(r));
            g->s150[0] = r;
        }
    }
    return g->s150[0];
}


void UpdateCarTrackNode(RaceVehicle *p)
{
  int s = 1;
  int t;
  int cur;
  int back;
  int fwd;
  int k;
  int j;
  int x;
  TrackSample3C *new_var;
  int n;
  TrackSample3C *base;
  int bk;
  t = FindTrackNodeUnbounded(p->position20.words, p->trackNodeF0);
  if (t < 0)
  {
    *(short *)&p->flags1E4 = -1;
    return;
  }
  if (ReverseCourse == 0)
  {
    if (t != p->trackNodeF0)
    {
      cur = p->trackNodeF0;
      n = TrackNodeCount;
      base = TrackNodes;
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
            p->first += new_var[j].segmentLength1C;
          }

          break;
        }
        if (t == fwd)
        {
          for (k = 1; s >= k; k++)
          {
            p->first -= new_var[(p->trackNodeF0 + k) % n].segmentLength1C;
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
    n = TrackNodeCount;
    base = TrackNodes;
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
          p->first += base[(p->trackNodeF0 + k) % n].segmentLength1C;
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
          p->first -= base[j].segmentLength1C;
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

void ClampAiCarLateralMotion(RaceVehicle *arg0) {
    s32 temp_v1;
    s32 var_v1;

    var_v1 = arg0->input * 3;
    if (var_v1 >= 0xC1) {
        var_v1 = 0xC0;
    }
    temp_v1 = arg0->lateralMotion178;
    arg0->wheelSteering17C = (arg0->wheelSteering17C - var_v1) & 0xFFF;
    if (temp_v1 >= 0x1000) {
        arg0->lateralMotion178 = 0x1000;
        return;
    }
    if (temp_v1 < -0xFFF) {
        arg0->lateralMotion178 = -0x1000;
    }
}

void UpdateCarGear(RaceVehicle *a0, int a1)
{
  RaceVehicleControl *q = &a0->control;
  T *t;
  q->pathDeltaZ34 = q->previewSample84.halves.high;
  t = (T *)q->record18;
  if (q->previewSample84.halves.low != 0)
  {
    int v = CarInputs[a1].b;
    if (v & 4)
    {
      if (q->previewSample84.halves.high < t->n)
      {
        if (q->laneTimer38 == 0)
        {
          q->previewSample84.halves.high = q->previewSample84.halves.high + 1;
          q->laneTimer38 = 10;
          q->reserved32 = 1;
          return;
        }
      }
    }
    else
      if (v & 8)
    {
      if (q->previewSample84.halves.high >= 2)
      {
        q->previewSample84.halves.high = q->previewSample84.halves.high - 1;
        q->laneTimer38 = 10;
      }
    }
  }
  else
  {
    int a;
    unsigned short u, u2;
    T *e1, *e2;
    if (u = q->previewSample84.halves.high, a0->input < (e1 = &t[q->previewSample84.halves.high])->b)
    {
      if (q->previewSample84.halves.high >= 2)
      {
        q->previewSample84.halves.high = u - 1;
        q->laneTimer38 = 10;
        goto L;
      }
    }
    if (u2 = q->previewSample84.halves.high, a0->input >= (e2 = &t[q->previewSample84.halves.high])->a)
    {
      if (q->laneTimer38 == 0)
      {
        if (q->previewSample84.halves.high < t->n)
        {
          q->previewSample84.halves.high = u2 + 1;
          q->laneTimer38 = 10;
          q->reserved32 = 1;
        }
      }
    }
    L:
    if (a0->input == 0)
    {
      if (q->previewSample84.halves.high >= 2)
      {
        if (q->dynamics94.player.motionState10 != 4)
        {
          q->previewSample84.halves.high = 1;
          q->laneTimer38 = 0;
        }
      }
    }

  }
}

int FindTrackNodeAtXZ(int a0, int a1)
{
    int sp[4];
    /* ASM_BARRIER: pin y to $a1 */
    register int y asm("$5");
    /* ASM_BARRIER: pin v to $v0 */
    register int v asm("$2");
    a0 >>= 2;
    y = a1 >> 2;
    sp[0] = a0;
    sp[2] = y;
    if (a0 < 0) a0 += 0x7FF;
    v = y;
    a0 >>= 11;
    if (v < 0) v += 0x7FF;
    a0 = a0 + ((v >> 11) << 5);
    if (a0 < 0) a0 = 0;
    else if (a0 >= 0x400) a0 = 0x3FF;
    return FindTrackNode(sp, TrackNodeGrid[a0], 6);
}
