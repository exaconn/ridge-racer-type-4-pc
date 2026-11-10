#ifndef RR_TRACK_SAMPLE_TYPES_H
#define RR_TRACK_SAMPLE_TYPES_H
#include "rr/layout.h"
/* Centre line, lateral offsets and road widths at one track sample. */
typedef struct {
    s16 rightOffsetX00, rightOffsetZ02;
    s16 leftOffsetX04, leftOffsetZ06;
    s32 x08, z0C;
    s16 y10,heading12;
    s16 unk14, pitch16;
    s16 leftWidth18, rightWidth1A;
    s16 segmentLength1C, angle1E;
    s16 angleLeft20, angleRight22;
    s32 dist24;
    s16 unk28[2];
    s16 rightExtension2C, leftExtension2E;
    s16 unk30[2];
    s16 unk34[2];
    s16 unk38[2];
} TrackSample3C;

/* Horizontal offsets of the selected driving line, one record per sample. */
typedef struct {
    s16 x, z;
    s32 reserved04;
} TrackLaneOffset8;

extern TrackSample3C *TrackNodes;
extern TrackLaneOffset8 *TrackLaneOffsets;
extern s32 TrackNodeCount;
extern s32 *TrackStartNode;

RR_ASSERT_SIZE(TrackSample3C, 0x3C);
RR_ASSERT_OFFSET(TrackSample3C,x08,0x08);
RR_ASSERT_OFFSET(TrackSample3C,z0C,0x0C);
RR_ASSERT_OFFSET(TrackSample3C,y10,0x10);
RR_ASSERT_OFFSET(TrackSample3C,leftOffsetX04,0x04);
RR_ASSERT_OFFSET(TrackSample3C,heading12,0x12);
RR_ASSERT_OFFSET(TrackSample3C,leftWidth18,0x18);
RR_ASSERT_OFFSET(TrackSample3C,rightExtension2C,0x2C);
RR_ASSERT_OFFSET(TrackSample3C,segmentLength1C,0x1C);
RR_ASSERT_SIZE(TrackLaneOffset8,8);
RR_ASSERT_OFFSET(TrackLaneOffset8,z,2);
#endif
