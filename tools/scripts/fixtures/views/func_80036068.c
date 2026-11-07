#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800AD508 (*(short (*))&D_800AD508)

#undef D_800AD508

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEBC (*(char (*)[])&D_800ABEBC)

#undef D_800ABEBC

/* legacy typing of shared globals in this part of the unit */
#define D_800AD508 (*(s16 (*))&D_800AD508)

#undef D_800AD508

/* legacy typing of shared globals in this part of the unit */
#define D_800F2DE4 (*(unsigned int (*))&D_800F2DE4)

#define D_800F3A26 (*(unsigned short (*))&D_800F3A26)

#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

#undef D_800F2DE4

#undef D_800F3A26

#undef D_800F4D8A

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

#define D_800F2DE4 (*(unsigned int (*))&D_800F2DE4)

#undef D_800F4D8A

#undef D_800F2DE4

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

#undef D_800F4D8A

/* legacy typing of shared globals in this part of the unit */
#define D_800F2DE4 (*(u32 (*))&D_800F2DE4)

#undef D_800F2DE4

/* legacy typing of shared globals in this part of the unit */
#define D_800AD4D8 (*(struct w3g2s (*))&D_800AD4D8)

#undef D_800AD4D8

extern int SoundSetMasterVolume();

extern s32 D_800F2EC8;

void func_80036068(void) {
    SoundSetMasterVolume(0, 0);
    func_80036030();
    D_800F2EC8 = 0;
}
