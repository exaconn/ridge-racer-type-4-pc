#ifndef RR_VEHICLE_DRIVE_H
#define RR_VEHICLE_DRIVE_H
#include "rr/race.h"
#include "psyq/libgte/gteatan.h"
/* Road sampler: writes the peak heading change and the left/right sums. */
s32 ScanCurveAhead(RaceVehicle *vehicle, s32 lane, s32 *peak, s32 *negative, s32 *positive);
void PlanComCarCornerTurn(RaceVehicle *vehicle);
void UpdateEngineRpm(RaceVehicle *vehicle);
/* Gear after a pending shift addresses one of six ratios. Pure angle/response
 * services retain the vehicle; road sampling writes its three output words.
 * Lane and final drive updates can change the complete live vehicle. Response
 * is signed16-normalized in a full word. Products wrap32 before signed division;
 * steering filtering narrows before clamping and before the averaging step. */
void UpdateComCarDrive(RaceVehicle *vehicle);
#endif
