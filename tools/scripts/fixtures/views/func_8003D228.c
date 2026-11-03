#include "rr/ui_panel.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC ((E *)D_800F3AFC)

#undef D_800F3AFC

/* legacy typing of shared globals in this part of the unit */
#define func_8001FF84 (*(RrSpritePacket *(*)(u32*,RrSpritePacket*,s32,s32,RrUiUvRect*,u16,s32))&func_8001FF84)

#define func_8001FEC0 (*(RrSpritePacket *(*)(u32*,RrSpritePacket*,s32,s32,s32,s32,s32,s32,s32))&func_8001FEC0)

#undef func_8001FF84

#undef func_8001FEC0

extern int D_800F2ECC;

void func_8003D228(a0, a1, a2, a3, a4, a5, a6)
int a0;
int a1;
short a2;
int a3;
unsigned short a4;
unsigned short a5;
unsigned short a6;
{
  int s;
  unsigned int z;
  if (a0 < D_800F2ECC)
  {
    D_800F3AFC[a0].a = a6;
    D_800F3AFC[a0].b = 0;
    s = (short) a6;
    if (s < 0)
    {
      a6 = 0;
    }
    else
    {
      z = s == 0;
      a6 = 0x16;
      if (z)
      {
        a4 = 0x80;
      }
    }
    D_800F3AFC[a0].d = a2 - a6;
    D_800F3AFC[a0].f = a2 - a6;
    D_800F3AFC[a0].c = a1;
    D_800F3AFC[a0].e = a1;
    D_800F3AFC[a0].g = a3;
    D_800F3AFC[a0].h = a4 + a6;
    D_800F3AFC[a0].i = a5;
  }
}
