/* MASPSX_FLAGS: --assembler-profile=gnu */

/* CC1_FLAGS: -mno-split-addresses */

#include "rr/draw_environment.h"

#include "rr/gpu.h"

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

/* legacy typing of shared globals in this part of the unit */
#define func_80096444 ((int (*)())func_80096444)

#undef func_80096444

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((void *)D_800ABAB8)

#undef D_800ABAB8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC8 ((s32 *)D_800ABBC8)

#undef D_800ABBC8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC8 ((u32 *)D_800ABBC8)

#undef D_800ABBC8

/* legacy typing of shared globals in this part of the unit */
#define D_800ABBC4 ((int *)D_800ABBC4)

#define D_800ABBC8 ((int *)D_800ABBC8)

#undef D_800ABBC4

#undef D_800ABBC8

u32 get_tw(RrGpuRectangleFields *a0)
{
  int v[4];
  int new_var;
  unsigned r;
  if (a0 == 0)
  {
    r = 0;
  }
  else
  {
    v[0] = ((u8 *)&a0->x)[0] >> 3;
    v[2] = ((-a0->w) & 0xFF) >> 3;
    v[1] = ((u8 *)&a0->y)[0] >> 3;
    v[3] = ((-a0->h) & 0xFF) >> 3;
    r = (((v[1] << 15) | ((v[0] << 10) | (new_var = 0xE2000000))) | (v[3] << 5)) | v[2];
  }
  return r;
}
