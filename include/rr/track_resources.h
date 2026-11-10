#ifndef RR_TRACK_RESOURCES_H
#define RR_TRACK_RESOURCES_H

#include "rr/lookup.h"
#include "rr/track_sample_types.h"

/* R4 track-resource directory. Section offsets are relative to this header. */
typedef struct {
    u32 reserved00;
    u32 lookupOffset04;
    u32 laneOffset08;
    u32 startNodeOffset0C;
    u32 workspaceOffset10;
} TrackResourceFile;

void RelocateTrackLookupTables(LookupTables32 *tables);
void SetTrackLaneOffsets(TrackLaneOffset8 *offsets);
void SetTrackStartNode(s32 *startNode);
void SetTrackWorkspace(u8 *workspace);
void LoadTrackResource(TrackResourceFile *file);

RR_ASSERT_SIZE(TrackResourceFile,0x14);
RR_ASSERT_OFFSET(TrackResourceContext,cameraNodes550,0x550);
RR_ASSERT_OFFSET(TrackResourceContext,workspace554,0x554);
RR_ASSERT_OFFSET(LookupTables32,auxiliary10,0x10);
RR_ASSERT_OFFSET(LookupTables32,auxiliary28,0x28);

#endif
