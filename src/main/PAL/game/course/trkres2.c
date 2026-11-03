#include "types.h"
#include "rr/steering.h"
#include "rr/track_resources.h"
#include "m2c_macros.h"
#include "game/course/trkres2.h"

extern s32 D_800ACB90;

void func_8004D354(RrCurve24 *curve, s32 displacement, s32 duration, s32 initialRate);
s32 func_8004D420(const RrCurve24 *curve, s32 time, s32 *output);
int func_8004D4BC(int a);
void func_8004D4DC(RrLookupTables32 *tables);
void func_8004D58C(RrTrackLaneOffset8 *offsets);
void func_8004D598(s32 *startNode);
void func_8004D5A4(u8 *workspace);
void func_8004D5CC(RrTrackResourceFile *file);

/* Build a two-phase rate curve in twelve-bit fixed-point units. */
void func_8004D354(RrCurve24 *curve, s32 displacement, s32 duration, s32 initialRate)
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

s32 func_8004D420(const RrCurve24 *curve, s32 time, s32 *output)
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

int func_8004D4BC(int a) {
    int v = a;
    int r = v & 3;
    if (r != 0) {
        v += 4 - r;
    }
    return v;
}

void func_8004D4DC(RrLookupTables32 *tables)
{
    D_800AC650.lookupTables = tables;
    tables->auxiliary10 = (void *)((unsigned long)tables->auxiliary10 + (unsigned long)tables);
    D_800AC650.lookupTables->auxiliary14 = (void *)((unsigned long)D_800AC650.lookupTables->auxiliary14 + (unsigned long)tables);
    D_800AC650.lookupTables->table0 = (RrTrackEventEntry *)((unsigned long)D_800AC650.lookupTables->table0 + (unsigned long)tables);
    D_800AC650.lookupTables->table1 = (RrTrackEventEntry *)((unsigned long)D_800AC650.lookupTables->table1 + (unsigned long)tables);
    D_800AC650.lookupTables->table2 = (RrTrackEventEntry *)((unsigned long)D_800AC650.lookupTables->table2 + (unsigned long)tables);
    D_800AC650.lookupTables->table3 = (RrTrackEventEntry *)((unsigned long)D_800AC650.lookupTables->table3 + (unsigned long)tables);
    D_800AC650.lookupTables->auxiliary28 = (void *)((unsigned long)D_800AC650.lookupTables->auxiliary28 + (unsigned long)tables);
}

void func_8004D58C(RrTrackLaneOffset8 *offsets)
{
    D_800ACB9C = offsets;
}

void func_8004D598(s32 *startNode)
{
    D_800ACB8C = startNode;
}

void func_8004D5A4(u8 *workspace)
{
    RrTrackResourceContext *context = &D_800AC650;
    context->workspace554 = workspace;
    workspace += 0xA00;
    context->workspace558 = workspace;
    workspace += 0x20;
    context->workspace55C = workspace;
    workspace += 0x200;
    context->cameraNodes550 = workspace;
}

void func_8004D5CC(RrTrackResourceFile *file)
{
    func_8004D4DC((RrLookupTables32 *)((u8 *)file + file->lookupOffset04));
    func_8004D58C((RrTrackLaneOffset8 *)((u8 *)file + file->laneOffset08));
    func_8004D598((s32 *)((u8 *)file + file->startNodeOffset0C));
    func_8004D5A4((u8 *)file + file->workspaceOffset10);
}

void func_8004D61C(s32 arg0) {
    D_800ACB90 = arg0;
}
