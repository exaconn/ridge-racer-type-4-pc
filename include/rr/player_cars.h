#ifndef RR_PLAYER_CARS_H
#define RR_PLAYER_CARS_H

#include "rr/race_types.h"

extern RaceVehicle PlayerCars[2];
/* Computer-controlled cars (0x80107198..0x801093F8, eleven records). The
 * versus and link course menus clear ComCars[0].position20 on entry. */
extern RaceVehicle ComCars[11];

#endif
