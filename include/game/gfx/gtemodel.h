#ifndef RR_GAME_GFX_GTEMODEL_H
#define RR_GAME_GFX_GTEMODEL_H

#include "types.h"

/* PAL label at CameraViewMatrix.fields.translation14, not separate storage.
 * The lit caller still needs this symbolic view for its matching address loads. */
extern s32 GteProjectionScaleTable[3];
void DrawTrackObjectModel(int a0, int a1);
void DrawTrackObjectLitModel(int placementIndex, int modelIndex);
void InitIntroSceneLights(void);
void BuildScaledCameraMatrices(void);

#endif /* RR_GAME_GFX_GTEMODEL_H */
