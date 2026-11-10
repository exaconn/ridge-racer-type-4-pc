#ifndef RR_LIGHTING_TYPES_H
#define RR_LIGHTING_TYPES_H

#include "rr/layout.h"
#include "rr/geometry.h"

typedef struct {u8 red,green,blue,pad03;} PreviewColor4;
typedef Vec3sFields8 PreviewVector8;
typedef struct {
    PreviewColor4 colors[3];
    PreviewVector8 vectors[3];
} PreviewLightInputFields36;
typedef union {
    PreviewLightInputFields36 fields;
    u16 halves[18];
    s16 signedHalves[18];
    u8 bytes[36];
} PreviewLightInput36;
typedef struct {
    PreviewLightInput36 matrix;
    u8 ambient[4];
} PreviewLighting40;

RR_ASSERT_SIZE(PreviewColor4,4);
RR_ASSERT_SIZE(PreviewVector8,8);
RR_ASSERT_SIZE(PreviewLightInputFields36,36);
RR_ASSERT_OFFSET(PreviewLightInputFields36,vectors,12);
RR_ASSERT_SIZE(PreviewLightInput36,36);
RR_ASSERT_SIZE(PreviewLighting40,40);
RR_ASSERT_OFFSET(PreviewLighting40,ambient,36);

#endif
