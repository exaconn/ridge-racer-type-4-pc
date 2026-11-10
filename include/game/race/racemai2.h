#ifndef RR_GAME_RACE_RACEMAI2_H
#define RR_GAME_RACE_RACEMAI2_H

#include "types.h"
#include "rr/race_types.h"
#include "rr/collision_work.h"

/* The resident table dispatches eleven stages over the same vehicle owner. */
typedef void (*CarUpdateStage)(RaceVehicle *vehicle);
extern CarUpdateStage CarUpdateStages[11];

void StepRaceFrame(unsigned int gameMode);
unsigned PointInQuad(int corner0, int corner1, int corner2, int corner3, int point);
u32 PointInQuadPtr(const CollisionPoint *corners, s32 point);
void ComputeQuadMidpoints(s32 carIndex);
s32 GetCarTrackLightIndex(RaceVehicle *vehicle);
void PlayTrackEventSfx(RaceVehicle *vehicle);
void UpdateRumbleStrip(RaceVehicle *p, int b);
void ResolveCarCollisions(RaceVehicle *arg0);
void CheckFinalLapEvent(RaceVehicle *vehicle);


#endif /* RR_GAME_RACE_RACEMAI2_H */
