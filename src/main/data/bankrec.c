#include "types.h"
#include "rr/record_banks.h"
#include "game/data/ldscn1.h"
#include "game/menu/menu13.h"
#include "game/data/bankrec.h"


void SetupRaceMemoryArena(s32 layout,s32 mode);

void SetupDialogueTable(void *buffer) {
    u8 *a = buffer;
    int i;
    unsigned short *p;
    u16 **q;
    DialoguePageCounts = a + 0x50;
    i = 0;
    q = DialogueScriptPtrs;
    p = (unsigned short *)a;
    do { *q++ = (u16 *)(a + 0x1B8 + *p++ * 2); i++; } while (i < 0x28);
    DialogueAssetTable = (s8 *)(a + 0x78);
}

void SetCarRecordPtr(CarRecord *record, s32 index) {
    CarRecordPtrs[index] = record;
}

void SetLightTable(TrackLighting2C *table) {
    TrackLightTable = table;
}

void SetupRaceMemoryArena(s32 layout,s32 mode) {
    s32 i;
    u32 factor;
    u8 *base;
    u8 *loaded;
    u8 *bankbase;
    RaceVehicle **source_slots;
    u32 offset;
    RaceVehicle **bank_source_slots;
    ReplayFrame4 **bank_slots;
    u32 bank_offset;
    RaceVehicle **restore_source,**restore_dest;
    s32 bound;
    u8 *read_page;
    u8 *bound_page;

    ReplayFrame=0;
    ReplayState=mode;
    if (mode==2) {
        /* PAL mapping: page-0x3A64 is the live signed count NumCars. */
        read_page=(u8 *)0x800B0000U;
        i=0;
        if (((s16 *)read_page)[-0x3A64/2]>0) {
            bound_page=read_page;
            restore_dest=CarTable;
            restore_source=CarSnapshots;
            do {
                **restore_dest=**restore_source;
                if ((s16)(*restore_dest)->flags1E4>=17) {
                    (*restore_dest)->flags1E4&=255;
                }
                restore_dest++;
                bound=((s16 *)bound_page)[-0x3A64/2];
                i++;
                restore_source++;
            } while (i<bound);
        }
    } else {
        SavedRaceState=0;
        if (layout==1) {
            i=0;
            loaded=SharedWorkArena;
            base=loaded;
            source_slots=CarSnapshots;
            offset=RR_FIELD_OFFSET(ReplayTwoStorage,snapshots);
            ReplayLength=2700;
            ReplayInputs[0]=(ReplayFrame4 *)base;
            ReplayInputs[1]=(ReplayFrame4 *)(base+sizeof(((ReplayTwoStorage*)0)->inputs[0]));
            do {
                *source_slots=(RaceVehicle *)(offset+
                                                (unsigned long)base);
                source_slots++;
                i++;
                offset+=sizeof(RaceVehicle);
            } while (i<4);
        } else if (layout==2) {
            ReplayLength=900;
            i=0;
            factor=sizeof(((ReplayLinkStorage*)0)->inputs[0])/sizeof(ReplayFrame4);
            bankbase=SharedWorkArena;
            bank_source_slots=CarSnapshots;
            bank_offset=RR_FIELD_OFFSET(ReplayLinkStorage,snapshots);
            bank_slots=ReplayInputs;
            do {
                u32 scaled=i*sizeof(ReplayFrame4);
                u32 product;
                product=factor*scaled;
                *bank_slots=(ReplayFrame4 *)(product+(unsigned long)bankbase);
                *bank_source_slots=(RaceVehicle *)(bank_offset+
                                                     (unsigned long)bankbase);
                bank_source_slots++;
                bank_offset+=sizeof(RaceVehicle);
                i++;
                bank_slots++;
            } while (i<4);
        } else {
            i=0;
            loaded=SharedWorkArena;
            base=loaded;
            source_slots=CarSnapshots;
            offset=RR_FIELD_OFFSET(ReplaySingleStorage,snapshots);
            ReplayLength=3600;
            ReplayInputs[0]=(ReplayFrame4 *)base;
            do {
                *source_slots=(RaceVehicle *)(offset+
                                                (unsigned long)base);
                source_slots++;
                i++;
                offset+=sizeof(RaceVehicle);
            } while (i<8);
        }
    }
}
