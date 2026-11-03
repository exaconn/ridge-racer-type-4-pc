#include "m2c_macros.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(Context650 (*))&D_800AC650)

#undef D_800AC650

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(int (*)[])&D_800AC650)

#define D_800AC6B0 (*(char (*)[])&D_800AC6B0)

#define D_800AC6D8 (*(int (*))&D_800AC6D8)

#define D_800ACC14 ((char *)D_800ACC14)

#undef D_800AC650

#undef D_800AC6B0

#undef D_800AC6D8

#undef D_800ACC14

extern int func_80096434();

extern M2C_UNK D_800AC6D8;

void func_80045830(void) {
    func_80096434(&D_800AC6D8, 0x20);
    M2C_FIELD(&D_800AC6D8, s32 *, 0x1C) = -1;
}
