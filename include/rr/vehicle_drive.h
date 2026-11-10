#ifndef RR_VEHICLE_DRIVE_H
#define RR_VEHICLE_DRIVE_H
#include "rr/race.h"
#include "psyq/libgte/gteatan.h"
/* Legacy road sampler consumes the complete vehicle through a char view and
 * writes three distinct word-sized output locals. */
s32 ScanCurveAhead(char *vehicle,s32 lane,s32 *,s32 *,s32 *);
void PlanComCarCornerTurn(RaceVehicle *vehicle);
void UpdateEngineRpm(RaceVehicle *vehicle);
/* Gear after a pending shift addresses one of six ratios. Pure angle/response
 * services retain the vehicle; road sampling writes its three output words.
 * Lane and final drive updates can change the complete live vehicle. Response
 * is signed16-normalized in a full word. Products wrap32 before signed division;
 * steering filtering narrows before clamping and before the averaging step. */
void UpdateComCarDrive(RaceVehicle *vehicle);
#endif
