#ifndef RR_GAME_DATA_BANKREC_H
#define RR_GAME_DATA_BANKREC_H

#include "rr/car_record_types.h"
#include "rr/track_lighting.h"

extern CarRecord *CarRecordPtrs[];

void SetupDialogueTable();
void SetCarRecordPtr(CarRecord *record, s32 index);
void SetLightTable(TrackLighting2C *table);
void SetupRaceMemoryArena(s32 layout,s32 mode);


#endif /* RR_GAME_DATA_BANKREC_H */
