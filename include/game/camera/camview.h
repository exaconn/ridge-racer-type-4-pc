#ifndef RR_GAME_CAMERA_CAMVIEW_H
#define RR_GAME_CAMERA_CAMVIEW_H

#include "types.h"
#include "rr/geometry.h"
#include "rr/camera_curve_tables.h"

void DrawGarageFloorReflection(void);
s32 FindCurveSegment(PairS16 *p, s32 a1);
s32 InterpCurveLinear(PairS16 *t, s32 idx, s32 x);
s32 InterpCurveEase(s32 a0);

#endif /* RR_GAME_CAMERA_CAMVIEW_H */
