#include "rr/track_resources.h"

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
