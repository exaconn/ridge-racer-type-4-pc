#include "rr/ui_panel.h"

/* legacy typing of shared globals in this part of the unit */
#define BadgeRows ((E *)BadgeRows)

#undef BadgeRows

/* legacy typing of shared globals in this part of the unit */
#define func_8001FF84 (*(RrSpritePacket *(*)(u32*,RrSpritePacket*,s32,s32,RrUiUvRect*,u16,s32))&func_8001FF84)

#define GameQueueSprite (*(RrSpritePacket *(*)(u32*,RrSpritePacket*,s32,s32,s32,s32,s32,s32,s32))&GameQueueSprite)

#undef func_8001FF84

#undef GameQueueSprite

extern int BadgeRowCount;

void InitBadgeRow(a0, a1, a2, a3, a4, a5, a6)
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
  if (a0 < BadgeRowCount)
  {
    BadgeRows[a0].a = a6;
    BadgeRows[a0].b = 0;
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
    BadgeRows[a0].d = a2 - a6;
    BadgeRows[a0].f = a2 - a6;
    BadgeRows[a0].c = a1;
    BadgeRows[a0].e = a1;
    BadgeRows[a0].g = a3;
    BadgeRows[a0].h = a4 + a6;
    BadgeRows[a0].i = a5;
  }
}
