#ifndef RR_VEHICLE_TABLE_H
#define RR_VEHICLE_TABLE_H

#include "rr/race_types.h"

/* Live vehicles and whole-vehicle replay snapshots share one pointer type. */
extern RaceVehicle *CarTable[8];
extern RaceVehicle *CarSnapshots[8];

#endif
