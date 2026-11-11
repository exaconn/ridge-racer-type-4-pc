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

/* Resolves the serialized directory into a host-owned TrackEventDirectory
 * and publishes it in RaceContext.lookupTables. */
void RelocateTrackLookupTables(const TrackEventDirectoryWire *tables);
void SetTrackLaneOffsets(TrackLaneOffset8 *offsets);
void SetTrackStartNode(Grid53C *startGrid);
void SetTrackWorkspace(u8 *workspace);
void LoadTrackResource(TrackResourceFile *file);

RR_ASSERT_SIZE(TrackResourceFile,0x14);

#endif
