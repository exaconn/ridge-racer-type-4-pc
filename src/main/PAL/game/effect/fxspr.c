#include "game/gfx/fontdraw.h"
#include "rr/gpu_address.h"
#include "scratchpad.h"
#include "rr/frame_storage_types.h"
#include "game/effect/fxspr.h"
#include "game/gfx/fadespr.h"
#include "rr/frame_loop.h"
#include "rr/trig.h"
#include "psyq/libc/rand.h"

typedef struct { u32 tag; u32 code; } Pkt8;

extern int D_800A2098[];
extern const char D_800A1FC8[13][16];
extern unsigned int D_80011258;
extern FadeRaceContext RaceContext;
void DrawIntroRippleSprites(void)
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

    u32 *head;
  x = (RaceContext.f440 / 4) + (rsin(FrameCounter * 128U) / 128);
  l[0] = (x - 16) / 4;
  y = (RaceContext.f440 / 2) + (rcos(FrameCounter * 128U) / 128);
  l[1] = (y - 16) / 4;
  for (i = 0; i < 2; i++)
  {
    if (l[i] <= 0)
    {
      l[i] = 1;
    }
  }

  h[0] = (PsyqRand() % 16) - 8;
  h[1] = (PsyqRand() % 6) - 3;
  p = *((unsigned char **) SCRATCHPAD);
  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 4; j++)
    {
      ax = (j / 2) + 0x9E;
      ay = ((j % 2) << 1) + 0xE4;
      p = AddTextSprites(&CurrentDrawBuf->buffer.ordering70.words[10], p, ax - D_800A2098[RaceContext.f438], h[i] + ay, D_800A1FC8[RaceContext.f438], 0x7F38, new_var = 1, (l[i] << new_var) | new_var);
    }

  }

  w = D_80011258;
  *((unsigned char **) SCRATCHPAD) = p;
  p[3] = 1;
    head = &CurrentDrawBuf->buffer.ordering70.words[10];
  ((Pkt8 *)p)->code = (w & 0x9FF) | 0xE1000200;
  *((unsigned int *) p) = ((*((unsigned int *) p)) & 0xFF000000) | (*head & 0xFFFFFF);
  *head = (*head & 0xFF000000) | (GpuDmaEncodeAddress(p) & 0xFFFFFF);
  *((unsigned char **) SCRATCHPAD) = p + 8;
}
