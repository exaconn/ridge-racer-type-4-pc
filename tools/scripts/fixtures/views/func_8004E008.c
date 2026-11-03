#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((int *)D_8010C1F0)

#define D_800AC650 (*(struct { char pad[0x24C]; char *p; char pad2[0x560 - 0x250]; char *r560; } (*))&D_800AC650)

#undef D_8010C1F0

#undef D_800AC650

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)

#undef D_8010C1F0

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)

#undef D_8010C1F0

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)

#undef D_8010C1F0

/* legacy typing of shared globals in this part of the unit */
#define D_8010C1F0 ((char *)D_8010C1F0)

#define D_8010C210 (*(int (*)[2])&D_8010C210)

#undef D_8010C1F0

#undef D_8010C210

extern int D_800A0220;

extern int D_8010C1F0;

extern int D_800A0224;

extern s32 D_800A0218;

extern s32 D_800A021C;

extern s32 D_800ACC18;

void func_8004E008(void) {
    D_800A021C = 0;
    D_800A0220 = 0;
    D_800A0224 = 0;
    D_800A0218 = 0;
    D_8010C1F0 = D_800ACC18;
}
