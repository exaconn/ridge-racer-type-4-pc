#ifndef RR_TRACK_SAMPLE_TYPES_H
#define RR_TRACK_SAMPLE_TYPES_H
#include "rr/layout.h"
/* Centre line, lateral offsets and road widths at one track sample. */
typedef struct {
    s16 rightOffsetX00, rightOffsetZ02;
    s16 leftOffsetX04, leftOffsetZ06;
    s32 x08, z0C;
    s16 y10,heading12;
    u8 reserved14[4];
    s16 leftWidth18, rightWidth1A;
    s16 segmentLength1C;
    u8 reserved1E[14];
    s16 rightExtension2C, leftExtension2E;
    u8 reserved30[12];
} RrTrackSample3C;

/* Horizontal offsets of the selected driving line, one record per sample. */
typedef struct {
    s16 x, z;
    s32 reserved04;
} RrTrackLaneOffset8;

extern RrTrackSample3C *D_800ABEA4;
extern RrTrackLaneOffset8 *D_800ACB9C;
extern s32 D_800ABECC;
extern s32 *D_800ACB8C;

RR_ASSERT_SIZE(RrTrackSample3C, 0x3C);
RR_ASSERT_OFFSET(RrTrackSample3C,x08,0x08);
RR_ASSERT_OFFSET(RrTrackSample3C,z0C,0x0C);
RR_ASSERT_OFFSET(RrTrackSample3C,y10,0x10);
RR_ASSERT_OFFSET(RrTrackSample3C,leftOffsetX04,0x04);
RR_ASSERT_OFFSET(RrTrackSample3C,heading12,0x12);
RR_ASSERT_OFFSET(RrTrackSample3C,leftWidth18,0x18);
RR_ASSERT_OFFSET(RrTrackSample3C,rightExtension2C,0x2C);
RR_ASSERT_OFFSET(RrTrackSample3C,segmentLength1C,0x1C);
RR_ASSERT_SIZE(RrTrackLaneOffset8,8);
RR_ASSERT_OFFSET(RrTrackLaneOffset8,z,2);
#endif
