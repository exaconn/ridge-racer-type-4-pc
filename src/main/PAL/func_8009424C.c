#include "rr/draw_environment.h"

u32 func_8009424C(RrGpuRectangleFields *a0)
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
