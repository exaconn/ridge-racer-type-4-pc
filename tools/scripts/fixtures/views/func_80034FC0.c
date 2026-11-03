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

extern short D_800F3A26;

extern short D_800F4C6A;

extern short D_800F4D8A;

extern int func_8009AEAC();

extern s16 D_800FF688;

int func_80034FC0(void) {
    if (D_800FF688 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 0x2D;
        D_800F4D8A = 0;
        func_8009AEAC(1, 1, 0);
        return 1;
    }
    return 0;
}
