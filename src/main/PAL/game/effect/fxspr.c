#include "scratchpad.h"
#include "game/effect/fxspr.h"
#include "game/gfx/drawbuf.h"

extern int rsin();
extern int rcos();
extern int func_80096464();
extern unsigned char *AddTextSprites();
extern int FrameCounter;
extern int D_800A2098[];
extern int D_800A1FC8[];
extern unsigned int D_80011258;
extern struct 
{
  char pad[0x438];
  int f438;
  char pad2[4];
  int f440;
} RaceContext;
void func_8005C4E0(void)
{
  int l[2];
  short h[2];
  int i;
  int j;
  int x;
  int y;
  int ax;
  int ay;
  int w;
  int new_var;
  unsigned char *p;
  char *d;
  x = (RaceContext.f440 / 4) + (rsin(FrameCounter << 7) / 128);
  l[0] = (x - 16) / 4;
  y = (RaceContext.f440 / 2) + (rcos(FrameCounter << 7) / 128);
  l[1] = (y - 16) / 4;
  for (i = 0; i < 2; i++)
  {
    if (l[i] <= 0)
    {
      l[i] = 1;
    }
  }

  h[0] = (func_80096464() % 16) - 8;
  h[1] = (func_80096464() % 6) - 3;
  p = *((unsigned char **) SCRATCHPAD);
  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 4; j++)
    {
      ax = (j / 2) + 0x9E;
      ay = ((j % 2) << 1) + 0xE4;
      p = AddTextSprites(CurrentDrawBuf + 0x98, p, ax - D_800A2098[RaceContext.f438], h[i] + ay, D_800A1FC8 + (RaceContext.f438 * 4), 0x7F38, new_var = 1, (l[i] << new_var) | new_var);
    }

  }

  w = D_80011258;
  *((unsigned char **) SCRATCHPAD) = p;
  p[3] = 1;
  d = CurrentDrawBuf;
  *((unsigned int *) (p + 4)) = (w & 0x9FF) | 0xE1000200;
  *((unsigned int *) p) = ((*((unsigned int *) p)) & 0xFF000000) | ((*((unsigned int *) (d + 0x98))) & 0xFFFFFF);
  *((unsigned int *) (d + 0x98)) = ((*((unsigned int *) (d + 0x98))) & 0xFF000000) | (((unsigned int) p) & 0xFFFFFF);
  *((unsigned char **) SCRATCHPAD) = p + 8;
}
