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
} RrTrackResourceFile;

void func_8004D4DC(RrLookupTables32 *tables);
void func_8004D58C(RrTrackLaneOffset8 *offsets);
void func_8004D598(s32 *startNode);
void func_8004D5A4(u8 *workspace);
void func_8004D5CC(RrTrackResourceFile *file);

RR_ASSERT_SIZE(RrTrackResourceFile,0x14);
RR_ASSERT_OFFSET(RrTrackResourceContext,cameraNodes550,0x550);
RR_ASSERT_OFFSET(RrTrackResourceContext,workspace554,0x554);
RR_ASSERT_OFFSET(RrLookupTables32,auxiliary10,0x10);
RR_ASSERT_OFFSET(RrLookupTables32,auxiliary28,0x28);

#endif
