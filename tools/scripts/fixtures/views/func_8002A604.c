#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F4CB0 (*(short (*))&D_800F4CB0)

#undef D_800F4CB0

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC68 (*(M2C_UNK (*))&D_800ACC68)

#define D_800F4CB0 (*(M2C_UNK (*))&D_800F4CB0)

#undef D_800ACC68

#undef D_800F4CB0

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC68 (*(S_ACC68 (*))&D_800ACC68)

#define D_800F4CB0 (*(S_F4CB0 (*))&D_800F4CB0)

#undef D_800ACC68

#undef D_800F4CB0

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(s32 (*))&D_800ACC14)

#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC14 (*(s32 (*))&D_800ACC14)

#define AddTilePrim ((s32 (*)())AddTilePrim)

#undef D_800ACC14

#undef AddTilePrim

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(E (*)[])&D_800F3A38)

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define PlayerCars (*(S_800ABEF8 (*)[])&PlayerCars)

#undef PlayerCars

typedef struct { unsigned char a, b, c, d; } S;

extern S D_800ACC68;

extern int func_80096434();

extern struct { char pad[0xC]; unsigned char a; unsigned char b; short c; } D_800F4CB0;

void ClearScreenOverlayState(void) {
    func_80096434(&D_800F4CB0, 0x20);
    func_80096434(&D_800ACC68, 0xC);
}
