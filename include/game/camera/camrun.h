#ifndef RR_GAME_CAMERA_CAMRUN_H
#define RR_GAME_CAMERA_CAMRUN_H

#include "rr/presentation_types.h"

extern CameraKey16 D_800A1AA0[][3];
extern CameraKey16 D_800A1B60[][2];

void InitCameraMotion(int selection);
void StepCameraScript(void);

#endif /* RR_GAME_CAMERA_CAMRUN_H */
