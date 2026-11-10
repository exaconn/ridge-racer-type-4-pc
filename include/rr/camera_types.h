#ifndef RR_CAMERA_TYPES_H
#define RR_CAMERA_TYPES_H

#include "rr/layout.h"
#include "rr/geometry.h"
#include "rr/lighting_types.h"

typedef struct {
    s32 x, y, z, reservedC;
} CameraKey16;

typedef struct {
    s32 phase00, mode04, key08;
    CameraKey16 *keys0C;
    s32 start10[3], reserved1C;
    s32 target20[3], reserved2C;
    s32 delta30[3], reserved3C;
    s32 current40[3], reserved4C;
    u16 counter50;
    s16 horizon52;
    Vector modelPosition54;
    s16 angles64[3], reserved6A;
    s32 modelScale6C;
} CameraMotion70;

typedef struct {
    s16 operation, argument;
    s32 duration;
} CameraScriptEntry8;

#endif
