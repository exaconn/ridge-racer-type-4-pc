#ifndef RR_GAME_COURSE_TRKINTP2_H
#define RR_GAME_COURSE_TRKINTP2_H
#include "rr/race_types.h"

#include "types.h"

extern unsigned char PasswordCipherKey[];
void GetTrackPointAhead(RaceVehicle *vehicle, s32 *output, s32 distance);
int FindNearestTrackEntry(RaceVehicle *vehicle);
void InitDemoComCars(RaceVehicle *car, const s32 *params);
void CarLocalToWorld(RaceVehicle *vehicle, const Vec3s8 *offset, Vector *output);
/* Upload a TIM image (CLUT and pixels) from loaded file data. */
void LoadTimImage(const void *tim);
/* Upload each TIM of a size-prefixed TIM list from loaded file data. */
void LoadTimList(const void *list);
int ValidateRecordPassword(unsigned char *bytes, unsigned char *chars);
int EncodeRecordPassword(int course, int car, int time, int kind, int branch, char *out);
int DecodeRecordPassword(int *course, int *car, int *time, int *kind, int *branch, const char *in);

#endif /* RR_GAME_COURSE_TRKINTP2_H */
