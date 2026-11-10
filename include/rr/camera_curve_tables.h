#ifndef RR_CAMERA_CURVE_TABLES_H
#define RR_CAMERA_CURVE_TABLES_H

#include "rr/geometry.h"

/* Four three-point curves followed by a thirteen-point steering curve;
 * each ends with a negative time. InitCameraState updates the first value
 * of the curve at ShowcaseCamDepthCurve. */
extern PairS16 CarCameraHeightOffsets[3], FinishCamFollowTargets[3], ShowcaseCamDepthCurve[3];
extern PairS16 FinishCamSplineWeights[3], IntroCamDollyKeyframes[13];

#endif
