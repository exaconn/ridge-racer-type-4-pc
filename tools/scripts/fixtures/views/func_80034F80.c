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

extern int _sio_control();

extern s16 D_800FF688;

void func_80034F80(void) {
    if (D_800FF688 == 2) {
        D_800FF688 = 3;
        _sio_control(1, 1, 2);
    }
}
