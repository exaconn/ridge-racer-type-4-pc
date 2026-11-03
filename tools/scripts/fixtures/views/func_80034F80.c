#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(S2 (*)[])&D_800ABEF8)

#define D_800F3A38 (*(S1 (*)[])&D_800F3A38)

#undef D_800ABEF8

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define func_8009AEAC ((unsigned int (*)())func_8009AEAC)

#define D_800F3A38 (*(T38 (*))&D_800F3A38)

#define D_800FF688 (*(unsigned short (*))&D_800FF688)

#undef func_8009AEAC

#undef D_800F3A38

#undef D_800FF688

/* legacy typing of shared globals in this part of the unit */
#define func_8009AEAC ((unsigned (*)())func_8009AEAC)

#undef func_8009AEAC

extern int func_8009AEAC();

extern s16 D_800FF688;

void func_80034F80(void) {
    if (D_800FF688 == 2) {
        D_800FF688 = 3;
        func_8009AEAC(1, 1, 2);
    }
}
