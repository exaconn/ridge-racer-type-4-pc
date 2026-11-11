#ifndef RR_RECORD_BANKS_H
#define RR_RECORD_BANKS_H
#include "rr/vehicle_table.h"
#include "rr/replay_types.h"
#include "rr/work_arena.h"

extern s32 ReplayFrame,ReplayState,SavedRaceState,ReplayLength;

/* Mode2 restores the live signed count of whole vehicles. Sources and
 * destinations must be identical or disjoint and slots must remain accessible.
 * Other modes select the two-player, link or single ReplayStorage layout.
 * Work memory must hold that layout with the native vehicle alignment. PAL's
 * maximum payload is0x60E8; host offsets/strides follow the actual pointer width. */
void SetupRaceMemoryArena(s32 layout,s32 mode);
#endif
