#ifndef RR_GAME_COURSE_TRKINTP_H
#define RR_GAME_COURSE_TRKINTP_H

#include "types.h"
#include "rr/race_types.h"

s32 GetSmoothedTrackHeading(int a, int b);
int AngleDiffAbs(int a0, int a1);
s32 AngleDiffSigned(s32 arg0, s32 arg1);
void ComputeStartDistance(RaceVehicle *vehicle, int direction);

#endif /* RR_GAME_COURSE_TRKINTP_H */
