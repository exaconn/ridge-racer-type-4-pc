#ifndef RR_GAME_COURSE_TRKINTP2_H
#define RR_GAME_COURSE_TRKINTP2_H
#include "rr/race_types.h"

#include "types.h"

extern unsigned char PasswordCipherKey[];
void GetTrackPointAhead(RaceVehicle *vehicle, s32 *output, s32 distance);
int FindNearestTrackEntry(RaceVehicle *vehicle);
void InitDemoComCars();
void CarLocalToWorld(RaceVehicle *vehicle, const Vec3s8 *offset, Vector *output);
void LoadTimImage(int *words);
void LoadTimList(int *words);

#endif /* RR_GAME_COURSE_TRKINTP2_H */
