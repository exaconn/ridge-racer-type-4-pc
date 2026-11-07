#include "types.h"
#include "rr/steering.h"
#include "rr/track_resources.h"
#include "m2c_macros.h"
#include "game/course/trkres2.h"

extern s32 CarSpecTable;

void BuildRateCurve(RrCurve24 *curve, s32 displacement, s32 duration, s32 initialRate);
s32 EvalRateCurve(const RrCurve24 *curve, s32 time, s32 *output);
int AlignUp4(int a);

/* Build a two-phase rate curve in twelve-bit fixed-point units. */
void BuildRateCurve(RrCurve24 *curve, s32 displacement, s32 duration, s32 initialRate)
{
    s32 half = duration / 2;
    s32 scaledDisplacement = displacement * 4096;
    s32 middleRate;
    s32 scaledInitialRate = initialRate * 4096;
    curve->initialRate = scaledInitialRate;
    middleRate = scaledDisplacement / half - (initialRate * 2048);
    curve->displacement = scaledDisplacement;
    curve->duration = duration;
    curve->middleRate = middleRate;
    curve->firstAcceleration = (middleRate - curve->initialRate) / half;
    curve->secondAcceleration = -middleRate / half;
}

s32 EvalRateCurve(const RrCurve24 *curve, s32 time, s32 *output)
{
    s32 half;
    s32 rate;
    if (time >= curve->duration) {
        *output = -(curve->secondAcceleration / 2);
        return -1;
    }
    half = curve->duration / 2;
    if (time < half) {
        rate = curve->initialRate + curve->firstAcceleration / 2 + curve->firstAcceleration * time;
    } else {
        time -= half;
        rate = curve->middleRate + curve->secondAcceleration / 2 + curve->secondAcceleration * time;
    }
    *output = rate;
    return 0;
}

int AlignUp4(int a) {
    int v = a;
    int r = v & 3;
    if (r != 0) {
        v += 4 - r;
    }
    return v;
}

void RelocateTrackLookupTables(RrLookupTables32 *tables)
{
    RaceContext.lookupTables = tables;
    tables->auxiliary10 = (void *)((unsigned long)tables->auxiliary10 + (unsigned long)tables);
    RaceContext.lookupTables->auxiliary14 = (void *)((unsigned long)RaceContext.lookupTables->auxiliary14 + (unsigned long)tables);
    RaceContext.lookupTables->table0 = (RrTrackEventEntry *)((unsigned long)RaceContext.lookupTables->table0 + (unsigned long)tables);
    RaceContext.lookupTables->table1 = (RrTrackEventEntry *)((unsigned long)RaceContext.lookupTables->table1 + (unsigned long)tables);
    RaceContext.lookupTables->table2 = (RrTrackEventEntry *)((unsigned long)RaceContext.lookupTables->table2 + (unsigned long)tables);
    RaceContext.lookupTables->table3 = (RrTrackEventEntry *)((unsigned long)RaceContext.lookupTables->table3 + (unsigned long)tables);
    RaceContext.lookupTables->auxiliary28 = (void *)((unsigned long)RaceContext.lookupTables->auxiliary28 + (unsigned long)tables);
}

void SetTrackLaneOffsets(RrTrackLaneOffset8 *offsets)
{
    TrackLaneOffsets = offsets;
}

void SetTrackStartNode(s32 *startNode)
{
    TrackStartNode = startNode;
}

void SetTrackWorkspace(u8 *workspace)
{
    RrTrackResourceContext *context = &RaceContext;
    context->workspace554 = workspace;
    workspace += 0xA00;
    context->workspace558 = workspace;
    workspace += 0x20;
    context->workspace55C = workspace;
    workspace += 0x200;
    context->cameraNodes550 = workspace;
}

void LoadTrackResource(RrTrackResourceFile *file)
{
    RelocateTrackLookupTables((RrLookupTables32 *)((u8 *)file + file->lookupOffset04));
    SetTrackLaneOffsets((RrTrackLaneOffset8 *)((u8 *)file + file->laneOffset08));
    SetTrackStartNode((s32 *)((u8 *)file + file->startNodeOffset0C));
    SetTrackWorkspace((u8 *)file + file->workspaceOffset10);
}

void SetCarSpecTable(s32 arg0) {
    CarSpecTable = arg0;
}
