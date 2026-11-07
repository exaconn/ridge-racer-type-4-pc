#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define PlayerCars (*(S2 (*)[])&PlayerCars)

#define PlayerControllers (*(S1 (*)[])&PlayerControllers)

#undef PlayerCars

#undef PlayerControllers

/* legacy typing of shared globals in this part of the unit */
#define _sio_control ((unsigned int (*)())_sio_control)

#define PlayerControllers (*(T38 (*))&PlayerControllers)

#define LinkState (*(unsigned short (*))&LinkState)

#undef _sio_control

#undef PlayerControllers

#undef LinkState

/* legacy typing of shared globals in this part of the unit */
#define _sio_control ((unsigned (*)())_sio_control)

#undef _sio_control

extern int _sio_control();

extern s16 LinkState;

void BeginLinkSync(void) {
    if (LinkState == 2) {
        LinkState = 3;
        _sio_control(1, 1, 2);
    }
}
