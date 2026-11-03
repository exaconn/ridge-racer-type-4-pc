#include "types.h"

#define ROW(i) ((RrRow60 *)((s32)D_800FFC14 + (i) * 0x3C))

/* legacy typing of shared globals in this part of the unit */
#define func_8001FCC8 (*(unsigned *(*)())&func_8001FCC8)

#define D_800ACC14 ((char *)D_800ACC14)

#undef func_8001FCC8

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define D_800FFC14 ((E *)D_800FFC14)

#undef D_800FFC14

/* legacy typing of shared globals in this part of the unit */
#define func_8001ECE8 ((void (*)())func_8001ECE8)

#define D_800F4D8C ((char *)D_800F4D8C)

#undef func_8001ECE8

#undef D_800F4D8C

/* legacy typing of shared globals in this part of the unit */
#define D_800AD520 (*(unsigned char (*))&D_800AD520)

#define D_800F2DE4 (*(unsigned (*))&D_800F2DE4)

#define D_800F3AFC ((char *)D_800F3AFC)

#define D_800FF68C ((signed char *)D_800FF68C)

#undef D_800AD520

#undef D_800F2DE4

#undef D_800F3AFC

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

extern RrRow60 *D_800FFC14;

extern unsigned short D_8009FDA0[];

void func_80041320(a0, a1, a2)
int a0;
int a1;
int a2;
{
  E *p;
  ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->e = D_8009FDA0[a1];
  if (a2 < (-0xFF))
  {
    ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->d = -a2;
    return;
  }
  switch (a2)
  {
    case 0:
      ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->c = 0x20;
      return;

    case 1:
      ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->c = 0x3E;
      return;

    case -1:
      return;

  }

}
