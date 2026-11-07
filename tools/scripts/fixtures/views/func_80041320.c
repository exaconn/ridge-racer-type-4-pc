#include "types.h"

#define ROW(i) ((RrRow60 *)((s32)MenuTextRows + (i) * 0x3C))

/* legacy typing of shared globals in this part of the unit */
#define AddDrawModePrim (*(unsigned *(*)())&AddDrawModePrim)

#define D_800ACC14 ((char *)D_800ACC14)

#undef AddDrawModePrim

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define MenuTextRows ((E *)MenuTextRows)

#undef MenuTextRows

/* legacy typing of shared globals in this part of the unit */
#define func_8001ECE8 ((void (*)())func_8001ECE8)

#define GlyphRows ((char *)GlyphRows)

#undef func_8001ECE8

#undef GlyphRows

/* legacy typing of shared globals in this part of the unit */
#define D_800AD520 (*(unsigned char (*))&D_800AD520)

#define D_800F2DE4 (*(unsigned (*))&D_800F2DE4)

#define BadgeRows ((char *)BadgeRows)

#define D_800FF68C ((signed char *)D_800FF68C)

#undef D_800AD520

#undef D_800F2DE4

#undef BadgeRows

#undef D_800FF68C

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    s8 unk3;
    u8 unk4[24];
    u8 unk1C[24];
    u16 unk34;
    u16 unk36;
    s16 unk38;
    s16 unk3A;
} RrRow60;

typedef struct 
{
  char pad[0x1C];
  char c;
  char pad2[0x17];
  short d;
  unsigned short e;
} E;

extern RrRow60 *MenuTextRows;

extern unsigned short D_8009FDA0[];

void func_80041320(a0, a1, a2)
int a0;
int a1;
int a2;
{
  E *p;
  ((E *) (((int) MenuTextRows) + (a0 * 0x3C)))->e = D_8009FDA0[a1];
  if (a2 < (-0xFF))
  {
    ((E *) (((int) MenuTextRows) + (a0 * 0x3C)))->d = -a2;
    return;
  }
  switch (a2)
  {
    case 0:
      ((E *) (((int) MenuTextRows) + (a0 * 0x3C)))->c = 0x20;
      return;

    case 1:
      ((E *) (((int) MenuTextRows) + (a0 * 0x3C)))->c = 0x3E;
      return;

    case -1:
      return;

  }

}
