#ifndef RR_GAME_CAR_CARMOVE1_H
#define RR_GAME_CAR_CARMOVE1_H

#include "types.h"
#include "rr/race_types.h"

extern short *TrackNodeGrid;
void FindAxleTrackNode(void);
void FindWheelTrackNodes(RaceVehicle *arg0);
void UpdateWheelBumps(RaceVehicle *a);
void SetTrackNodeData(char *a);

#endif /* RR_GAME_CAR_CARMOVE1_H */
