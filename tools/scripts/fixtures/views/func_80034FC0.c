#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define PlayerCars (*(S2 (*)[])&PlayerCars)

#define D_800F3A38 (*(S1 (*)[])&D_800F3A38)

#undef PlayerCars

#undef D_800F3A38

/* legacy typing of shared globals in this part of the unit */
#define _sio_control ((unsigned int (*)())_sio_control)

#define D_800F3A38 (*(T38 (*))&D_800F3A38)

#define D_800FF688 (*(unsigned short (*))&D_800FF688)

#undef _sio_control

#undef D_800F3A38

#undef D_800FF688

/* legacy typing of shared globals in this part of the unit */
#define _sio_control ((unsigned (*)())_sio_control)

#undef _sio_control

extern short D_800F3A26;

extern short D_800F4C6A;

extern short D_800F4D8A;

extern int _sio_control();

extern s16 D_800FF688;

int func_80034FC0(void) {
    if (D_800FF688 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 0x2D;
        D_800F4D8A = 0;
        _sio_control(1, 1, 0);
        return 1;
    }
    return 0;
}
