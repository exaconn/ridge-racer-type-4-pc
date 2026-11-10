#ifndef RR_CAMERA_CURVE_TABLES_H
#define RR_CAMERA_CURVE_TABLES_H

#include "rr/geometry.h"

/* Four three-point curves followed by a thirteen-point steering curve;
 * each ends with a negative time. InitCameraState updates the first value
 * of the curve at D_800A0D7C. */
extern PairS16 D_800A0D64[3], D_800A0D70[3], D_800A0D7C[3];
extern PairS16 D_800A0D88[3], D_800A0D94[13];

#endif
