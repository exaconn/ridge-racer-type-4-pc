#include "rr/race_front_search.h"

/* Keep the original table-relative cursors, while using the canonical native
 * record stride and array offset (the pointer-bearing row grows on a host). */
#define RR_STANDING_PREFIX_SIZE \
    (sizeof(RrRaceStandingsTable)-sizeof(((RrRaceStandingsTable*)0)->entries18))
#define RR_STANDING_ROW(base) \
    ((RrRaceStandingEntry0C*)((base)+RR_STANDING_PREFIX_SIZE))

s32 FindNearestCarAhead(RrRaceVehicle *vehicle) {
    s32 best=-1,index=0,selected,limit,count,fresh_count,id;
    s32 first_count,second_count,distance,half,negative_half;
    /* ASM_BARRIER: keep the captured track length in the consumed mode register. */
    register s32 circumference __asm__("$5");
    RrRaceStandingsTable *table;
    /* ASM_BARRIER: retain the second pass's table-relative cursor. */
    register u8 *cursor __asm__("$9");
    u8 *reference;
    RrRaceDistanceContextPrefix *context;
    s32 flags;
    /* ASM_BARRIER: retain context mode until the initial identifier is formed. */
    register s32 mode __asm__("$5");
    s32 input;
    u8 *lookup_cursor;
    context=&RaceContext;
    flags=vehicle->flags1E4;
    mode=context->mode;
    input=vehicle->input;
    table=(RrRaceStandingsTable*)context->result;
    flags=(flags&15)+best;
    input++;
    limit=input*16;
    count=NumCars;
    selected=(s32)((u32)flags+(u32)mode);
    if(count>0) {
        first_count=count;
        lookup_cursor=(u8*)table;
lookup:
        if(selected==RR_STANDING_ROW(lookup_cursor)->unk00)selected=index;
        else {
            index++;
            lookup_cursor+=sizeof(RrRaceStandingEntry0C);
            if(index<first_count)goto lookup;
        }
    }
    /* ASM_BARRIER: preserve the original second count capture after lookup. */
    __asm__("" : : : "memory");
    index=0;
    fresh_count=NumCars;
    if(fresh_count>0) {
        reference=(u8*)table+selected*sizeof(RrRaceStandingEntry0C);
        second_count=fresh_count;
        circumference=TrackLength;
        cursor=(u8*)table;
        half=circumference/2;
        negative_half=(s32)(0U-(u32)circumference)/2;
        do {
            if(index!=selected) {
                id=RR_STANDING_ROW(cursor)->unk00;
                distance=table->entries18[id].totalDist04;
                distance=(s32)((u32)distance-(u32)RR_STANDING_ROW(reference)->totalDist04);
                distance%=circumference;
                if(distance>=half)distance=(s32)((u32)distance-(u32)circumference);
                if(distance<negative_half)distance=(s32)((u32)distance+(u32)circumference);
                if(distance>0&&distance<limit) {
                    best=id;
                    limit=distance;
                    table->gap14=distance;
                }
            }
            index++;
            cursor+=sizeof(RrRaceStandingEntry0C);
        }while(index<second_count);
    }
    return best;
}
