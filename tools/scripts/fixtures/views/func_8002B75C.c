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

#define func_8001FD20 ((s32 (*)())func_8001FD20)

#undef D_800ACC14

#undef func_8001FD20

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(E (*)[])&D_800F3A38)

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(S_800ABEF8 (*)[])&D_800ABEF8)

#undef D_800ABEF8

void func_8002B75C(char *d, char *s)
{
    int i = 0;
    char *p;
    if (*s) {
        do {
            d[i] = *s++;
            i++;
        } while (*s);
    }
    while (*(p = d + i++)) {
        *p = 0;
    }
}
